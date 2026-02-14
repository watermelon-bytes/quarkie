### The Quarkie file system on-disk layout

1. First sectors are borrowed by the so-called superblock which stores a common information about the filesystem and must be signed.
2. The superblock is followed by qnodes bitmap (QB below). Each bit in QB shows whether the matching slot is present.
3. Qnodes slots (QNS below) are placed next to QB. Qnodes hold common unit information: data sectors, names, etc.
4. The global free space bitmap (GFSB).
5. The rest of the block device's memory is split to *n* subareas of user data. Before every subarea there is a metadata sector, which stores ranges of available space in the subarea described by metadata. If the subarea, belonging to this metadata sector, has free sectors, then the matching bit in the GFSB is enabled.
