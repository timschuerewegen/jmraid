# jmraid
JMicron RAID Library

FANTEC QB-X2US3R output:

```
JMicron RAID info

Check "\\.\PhysicalDrive1" ...

  Get chip info ...

    Firmware version = 20.01.01.06
    Manufacturer     = HOTWAY
    Product name     = HOTWAY H/W RAID
    Serial number    = 427491329

  Get SATA info ...

    SATA Port 0

      Model name        = WDC WD20EFRX-68EUZN0
      Serial number     =      WD-WCC4M3NFRNP6
      Capacity          = 1863.00 GB
      Port type         = 2 (RAID Disk)
      Port speed        = 3 (Gen 3)
      Page 0 state      = 2 (Hooked to PM)
      RAID index        = 0
      RAID member index = 0
      Port              = 15

    SATA Port 1

      Model name        = WDC WD20EFRX-68EUZN0
      Serial number     =      WD-WCC4M0JTYKN6
      Capacity          = 1863.00 GB
      Port type         = 2 (RAID Disk)
      Port speed        = 3 (Gen 3)
      Page 0 state      = 2 (Hooked to PM)
      RAID index        = 0
      RAID member index = 1
      Port              = 15

    SATA Port 2

      Port type = 6 (Off)

    SATA Port 3

      Port type = 7 (Host)

    SATA Port 4

      Port type = 0 (No Device)

  Get SATA port 0 info ...

    Model name        = WDC WD20EFRX-68EUZN0
    Serial number     =      WD-WCC4M3NFRNP6
    Firmware version  = 82.00A82
    Capacity          = 1863.00 GB
    Capacity used     = 1862.97 GB
    Port type         = 2 (RAID Disk)
    Port              = 0
    Page 0 state      = 2 (Hooked to PM)
    RAID index        = 0
    RAID member index = 0

  Get SATA port 1 info ...

    Model name        = WDC WD20EFRX-68EUZN0
    Serial number     =      WD-WCC4M0JTYKN6
    Firmware version  = 82.00A82
    Capacity          = 1863.00 GB
    Capacity used     = 1862.97 GB
    Port type         = 2 (RAID Disk)
    Port              = 0
    Page 0 state      = 2 (Hooked to PM)
    RAID index        = 0
    RAID member index = 1

  Get SATA port 2 info ...

    Port type = 6 (Off)

  Get SATA port 3 info ...

    Port type = 6 (Off)

  Get SATA port 4 info ...

    Port type = 6 (Off)

  Get RAID port 0 info ...

    Model name       = H/W RAID1
    Serial number    = EDB1BFFDF37MG3M1G15C
    Port state       = 1
    Level            = 1 (RAID 1)
    Capacity         = 1862.97 GB
    State            = 3 (Normal)
    Member count     = 2
    Rebuild priority = 4096 (Medium)
    Standby timer    = 900 sec
    Password         =
    Rebuild progress = 0.00 %

    Member 0

      Ready         = 1
      LBA48 support = 1
      SATA port     = 0
      SATA page     = 0
      SATA base     = 0
      SATA size     = 1862.97 GB

    Member 1

      Ready         = 1
      LBA48 support = 1
      SATA port     = 1
      SATA page     = 0
      SATA base     = 0
      SATA size     = 1862.97 GB

  Get RAID port 1 info ...

    Port state = 0

  Get RAID port 2 info ...

    Port state = 0

  Get RAID port 3 info ...

    Port state = 0

  Get RAID port 4 info ...

    Port state = 0

  Get SMART info (disk 0) ...

      1 | 002F |  51 | 200 | 200 | 000000000000 |               0 | Raw Read Error Rate
      3 | 0027 |  21 | 178 | 172 | 000000000FFB |            4091 | Spin Up Time
      4 | 0032 |   0 | 100 | 100 | 000000000035 |              53 | Start/Stop Count
      5 | 0033 | 140 | 200 | 200 | 000000000000 |               0 | Reallocated Sectors Count
      7 | 002E |   0 | 200 | 200 | 000000000000 |               0 | Seek Error Rate
      9 | 0032 |   0 | 100 | 100 | 000000000033 |              51 | Power-On Time Count
     10 | 0032 |   0 | 100 | 253 | 000000000000 |               0 | Spin Retry Count
     11 | 0032 |   0 | 100 | 253 | 000000000000 |               0 | Drive Calibration Retry Count
     12 | 0032 |   0 | 100 | 100 | 00000000002E |              46 | Drive Power Cycle Count
    192 | 0032 |   0 | 200 | 200 | 000000000008 |               8 | Power off Retract Cycle
    193 | 0032 |   0 | 200 | 200 | 00000000005E |              94 | Load/Unload Cycle Count
    194 | 0022 |   0 | 115 | 109 | 000000000020 |              32 | HDD Temperature
    196 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Reallocation Event Count
    197 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Current Pending Sector Count
    198 | 0030 |   0 | 100 | 253 | 000000000000 |               0 | Off-Line Uncorrectable Sector Count
    199 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Ultra ATA CRC Error Count
    200 | 0008 |   0 | 100 | 253 | 000000000000 |               0 | Write Error Rate

  Get SMART info (disk 1) ...

      1 | 002F |  51 | 200 | 200 | 000000000000 |               0 | Raw Read Error Rate
      3 | 0027 |  21 | 175 | 173 | 000000001081 |            4225 | Spin Up Time
      4 | 0032 |   0 | 100 | 100 | 000000000032 |              50 | Start/Stop Count
      5 | 0033 | 140 | 200 | 200 | 000000000000 |               0 | Reallocated Sectors Count
      7 | 002E |   0 | 200 | 200 | 000000000000 |               0 | Seek Error Rate
      9 | 0032 |   0 | 100 | 100 | 000000000034 |              52 | Power-On Time Count
     10 | 0032 |   0 | 100 | 253 | 000000000000 |               0 | Spin Retry Count
     11 | 0032 |   0 | 100 | 253 | 000000000000 |               0 | Drive Calibration Retry Count
     12 | 0032 |   0 | 100 | 100 | 00000000002B |              43 | Drive Power Cycle Count
    192 | 0032 |   0 | 200 | 200 | 000000000005 |               5 | Power off Retract Cycle
    193 | 0032 |   0 | 200 | 200 | 000000000065 |             101 | Load/Unload Cycle Count
    194 | 0022 |   0 | 116 | 109 | 00000000001F |              31 | HDD Temperature
    196 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Reallocation Event Count
    197 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Current Pending Sector Count
    198 | 0030 |   0 | 100 | 253 | 000000000000 |               0 | Off-Line Uncorrectable Sector Count
    199 | 0032 |   0 | 200 | 200 | 000000000000 |               0 | Ultra ATA CRC Error Count
    200 | 0008 |   0 | 100 | 253 | 000000000000 |               0 | Write Error Rate

Check "\\.\PhysicalDrive2" ...

  jmraid_open failed

...
```
