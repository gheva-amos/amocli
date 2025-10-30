// Backup a directory to a tar file
// Assumes the backed up folder is in a path based on the date of the backup
// THe format of the directory is /ROOT/YYYY/MM/DD
// Will create a tar file named YYYYMMDD.tgz

package main

import (
  "fmt"
  "flag"
  "strings"
  "os/exec"
)

func main() {
  root_dir := flag.String("r", "/Volumes/Backups", "The root directory under which we will find the folder to backup")
  year := flag.Int("y", 2025, "The year we are backing up")
  month := flag.Int("m", 10, "The month we are backing up")
  day := flag.Int("d", 10, "The day we are backing up")

  flag.Parse()

  cmd := fmt.Sprintf("czf %d%d%d.tgz %s/%d/%d/%d", *year, *month, *day, *root_dir, *year, *month, *day)
  cmd_array := strings.Fields(cmd)

  command := exec.Command("tar", cmd_array...)
  if err := command.Run(); err != nil {
    fmt.Println(err)
    return
  }
}
