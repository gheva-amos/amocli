// Backup a directory to a tar file
// Assumes the backed up folder is in a path based on the date of the backup
// THe format of the directory is /ROOT/YYYY/MM/DD
// Will create a tar file named YYYYMMDD.tgz

package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func main() {
	root_dir := flag.String("r", "/Volumes/Backups", "The root directory under which we will find the folder to backup")
	year := flag.Int("y", 0, "The year we are backing up")
	month := flag.Int("m", 0, "The month we are backing up")
	day := flag.Int("d", 0, "The day we are backing up")
	config := flag.String("c", "", "the config file for amocli utils")
	fmt.Println(*config)

	flag.Parse()

	var cfg map[string]interface{}
	var appCfg map[string]interface{}
	cfgPath := cfgFilePath(*config)
	fmt.Println(cfgPath)
	if _, err := os.Stat(cfgPath); err == nil {
		data, err := os.ReadFile(cfgPath)
		if err != nil {
			panic(err)
		}
		if err := json.Unmarshal(data, &cfg); err != nil {
			panic(err)
		}
		appCfg = subMap(cfg, "bu")
	} else if os.IsNotExist(err) {
		cfg = make(map[string]interface{})
		appCfg = subMap(cfg, "bu")
		appCfg["year"] = 2025
		appCfg["month"] = 10
		appCfg["day"] = 15
	} else {
		panic(err)
	}

	intCfgValue(year, "year", appCfg)
	intCfgValue(month, "month", appCfg)
	intCfgValue(day, "day", appCfg)

	cmd := fmt.Sprintf("czf %d%d%d.tgz %s/%d/%d/%d", *year, *month, *day, *root_dir, *year, *month, *day)
	cmd_array := strings.Fields(cmd)

	command := exec.Command("tar", cmd_array...)
	if err := command.Run(); err != nil {
		fmt.Println(err)
		return
	}
}

func subMap(m map[string]interface{}, key string) map[string]interface{} {
	sub, ok := m[key].(map[string]interface{})
	if !ok {
		sub = make(map[string]interface{})
		m[key] = sub
	}
	return sub
}

func intCfgValue(val *int, key string, cfg map[string]interface{}) {
	if *val == 0 {
		switch tmp := cfg[key].(type) {
		case int:
			*val = tmp
		case float64:
			*val = int(tmp)
		default:
			panic(fmt.Sprintf("config key %q is not an int", key))
		}
	}
}

func cfgFilePath(cfgPath string) string {
	if cfgPath != "" {
		return cfgPath
	}
	home, err := os.UserHomeDir()
	if err != nil {
		return "error"
	}
	path := filepath.Join(home, ".amocli.json")
	return path
}
