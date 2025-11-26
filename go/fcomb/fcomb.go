package main

import (
	"crypto/sha256"
	"encoding/hex"
	"flag"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
)

type Directory struct {
	Files []File
}

type File struct {
	Path string
	Hash string
}

type UniqueFile struct {
	Root []string
	Hash []string
}

type Copy struct {
	From string
	To   string
}

func (f File) String() string {
	return fmt.Sprintf("%s:%s", f.Path, f.Hash)
}

func (d Directory) String() string {
	var sb strings.Builder
	for _, f := range d.Files {
		sb.WriteString(fmt.Sprintf("%s\n", f))
	}
	return sb.String()
}

func main() {
	test_run := flag.Bool("dry_run", false, "just list what you will be doing without touching the files")
	out := flag.String("out", "", "The directory into which we will copy everything")
	flag.Parse()
	dirs := flag.Args()

	if *out == "" {
		os.Exit(2)
	}

	var all_files map[string]Directory
	all_files = make(map[string]Directory)
	for _, dir := range dirs {
		tmp := Directory{}
		err := WalkDirectory(dir, func(path string, d fs.DirEntry) error {
			if !d.IsDir() {
				hash, err := FileSHA256(path)
				if err != nil {
					panic(err)
				}
				tmp.Files = append(tmp.Files, File{Path: strings.TrimPrefix(path, dir), Hash: hash})
			}
			return nil
		})
		all_files[dir] = tmp
		if err != nil {
			panic(err)
		}
	}
	unique_files := make(map[string]UniqueFile)
	for dir, strct := range all_files {
		for _, file := range strct.Files {
			p, ok := unique_files[file.Path]
			if !ok {
				tmp := UniqueFile{}
				tmp.Root = append(tmp.Root, dir)
				tmp.Hash = append(tmp.Hash, file.Hash)
				unique_files[file.Path] = tmp
			} else {
				found := false
				for _, hash := range p.Hash {
					if file.Hash == hash {
						found = true
					}
				}
				if !found {
					p.Root = append(p.Root, dir)
					p.Hash = append(p.Hash, file.Hash)
					unique_files[file.Path] = p
				}
			}
		}
	}
	var workorder []Copy
	for path, uf := range unique_files {
		if len(uf.Root) != 1 {
			for i, root := range uf.Root {
				workorder = append(workorder, Copy{From: filepath.Join(root, path), To: AddNumber(filepath.Join(*out, path), i)})
			}
		} else {
			workorder = append(workorder, Copy{From: filepath.Join(uf.Root[0], path), To: filepath.Join(*out, path)})
		}
	}

	for _, cp := range workorder {
		fmt.Printf("Copying %s to %s\n", cp.From, cp.To)
      if *test_run {
        continue
      }
    CopyFile(cp.From, cp.To)
	}
}

func WalkDirectory(root string, fn func(path string, d fs.DirEntry) error) error {
	return filepath.WalkDir(root, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		return fn(path, d)
	})
}

func FileSHA256(path string) (string, error) {
	f, err := os.Open(path)
	if err != nil {
		return "", err
	}
	defer f.Close()

	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		return "", err
	}
	return hex.EncodeToString(h.Sum(nil)), nil
}

func AddNumber(path string, number int) string {
	dir := filepath.Dir(path)
	base := filepath.Base(path)
	ext := filepath.Ext(base)
	name := base[:len(base)-len(ext)]
	suffix := fmt.Sprintf("_%d", number)
	return filepath.Join(dir, name+suffix+ext)
}

func CopyFile(src, dst string) error {
	if err := os.MkdirAll(filepath.Dir(dst), 0755); err != nil {
		return err
	}
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()

	stat, err := in.Stat()
	if err != nil {
		return err
	}

	out, err := os.OpenFile(dst, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, stat.Mode())
	if err != nil {
		return err
	}
	defer out.Close()

	_, err = io.Copy(out, in)
	if err != nil {
		return err
	}

	return out.Sync()
}
