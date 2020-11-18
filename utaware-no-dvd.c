/* 
 * This file is part of utaware-no-dvd
 * Copyright (c) 2020 bsdf
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>

int
check_exe(FILE *f)
{
  if (fseek(f, 0x134f, SEEK_SET) != 0)
    {
      perror("error seeking to offset");
      return 0;
    }
    
  uint8_t buf[2];
  if (fread(buf, sizeof(uint8_t), 2, f) != 2
      || buf[0] != 0x33
      || buf[1] != 0xc0)
    {
      fprintf(stderr, "wrong bytes found at offset\n");
      return 0;
    }

  if (fseek(f, -2, SEEK_CUR) != 0)
    {
      perror("error seeking to offset");
      return 0;
    }

  return 1;
}

int
main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "wrong number of args\n");
      fprintf(stderr, "usage: %s [utaware-dvd.exe]\n", argv[0]);
      return 1;
    }
  
  FILE *f = fopen(argv[1], "r+b");
  if (!f)
    {
      perror("could not open file");
      return 1;
    }

  if (!check_exe(f))
    {
      fclose(f);
      return 1;
    }

  uint8_t patch[2] = { 0xeb, 0x15 };
  if (fwrite(patch, sizeof(uint8_t), 2, f) != 2)
    {
      perror("error patching offset");
      fclose(f);
      return 1;
    }

  fclose(f);
  printf("%s patched.\n", argv[1]);
  return 0;
}
