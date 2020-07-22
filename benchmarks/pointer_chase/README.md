`pointer_generator.c`

creates multiple LinkedList of disk pointers with 3 tuples and writes them to dp.dat 
(diskpointer.dat)
all head pointers of each LinkedList are stored in hp.dat
(headpointer.dat)

@parm: num_lists
@parm: max_pointer_value - maximum disk block number
@parm: max_list_length, @parm:min_list_length
:pointer lists are randomly generated but limited by max and min list length

eg.
source_block	destination_block	list_length
65	45	3
45	22	2
22	END_BLOCK_MAGIC	1

this shows a list of 3 pointer blocks, where block head - 65 points to block 45, and the 
next two records store the remaining linked list of blocks. END_BLOCK_MAGIC is a marker
for verification purposes to check if last block was correctly reached.

`write_pointer_to_disk.c`

writes blocks to the underlying device with pointers generated by the pointer_generator.
each blocks' pointer is stored at offset 0 in each disk block. The last block does not point
to any block, we add a END_BLOCK_MAGIC to the block which can be used to verify if we have
reached the last block while traversing a LL.

`host_pointer_reader.c`

a program that runs at the host (inside the guest VM) and repeatedly issues read requests
for blocks to the underlying disk. it reads all pointer "heads" from dp.dat and
reads these blocks, and chases the pointers stored in each block until no more pointers are
present.

`nsc_pointer_reader.c`
issues read to the head of the linked list (inside the guest VM) and receives the tail of
the linked list. This requires compute_mode=1 and hardcoded values in NSC Process.

scripts/output - the folder contain benchmarks for pointer_chase in different configurations.
Example Workflow:

1. Run `pointer_generator` eg.:
	`./pointer_generator 10 1024 2 5
	This generates 10 pointers with 2 to 5 linkedlist length, with unique disk blocks numbered
	between 0 to 1023. The created linked lists are written on hp.dat and dp.dat.

2. Run `write_pointer_to_disk` eg.:
	`./write_pointer_to_disk /dev/nvme0n1`
	This writes all pointers in dp.dat to the disk at different offsets, with each disk block
	storing the next pointer in the first 8 bytes (`uint64_t`).

3. Run one of the two scripts `host_pointer_reader` or `nsc_pointer_reader` eg.:
	`./host_pointer_reader /dev/nvme0n1`
	This generates `host_time` or `nsc_time` files containing the number of CPU cycles spent
	while reading the pointer from disk.