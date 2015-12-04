#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)

#define max(a,b)			\
	({ __typeof__ (a) _a = (a);	\
	   __typeof__ (b) _b = (b);	\
		_a > _b ? _a : _b; })

#define min(a,b)			\
	({ __typeof__ (a) _a = (a);	\
	   __typeof__ (b) _b = (b);	\
		_a < _b ? _a : _b; })

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* struct boot_params has struct setup_header at 0ffset 0x1f1 */
struct __attribute__((__packed__)) setup_header {
        uint8_t         _pad0[0x1f1];
        uint8_t         setup_sects;
        uint16_t        root_flags;
        uint32_t        syssize;
        uint16_t        ram_size;
        uint16_t        vid_mode;
        uint16_t        root_dev;
        uint16_t        boot_flag;
        uint16_t        jump;
        uint32_t        header;
#define HDR_MAGIC               "HdrS"
#define HDR_MAGIC_SZ    4
        uint16_t        version;
#define VERSION(h,l)    (((h)<<8) | (l))
        uint32_t        realmode_swtch;
        uint16_t        start_sys;
        uint16_t        kernel_version;
        uint8_t         type_of_loader;
        uint8_t         loadflags;
        uint16_t        setup_move_size;
        uint32_t        code32_start;
        uint32_t        ramdisk_image;
        uint32_t        ramdisk_size;
        uint32_t        bootsect_kludge;
        uint16_t        heap_end_ptr;
        uint16_t        _pad1;
        uint32_t        cmd_line_ptr;
        uint32_t        initrd_addr_max;
        uint32_t        kernel_alignment;
        uint8_t         relocatable_kernel;
        uint8_t         _pad2[3];
        uint32_t        cmdline_size;
        uint32_t        hardware_subarch;
        uint64_t        hardware_subarch_data;
        uint32_t        payload_offset;
        uint32_t        payload_length;
};

void usage(char *argv[])
{
	fprintf(stderr, "Usage: %s <kernel-bzimage>\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	FILE *f;
	struct stat sb;
	int ret = -EINVAL;;
	const char *kernel_filename;
	uint8_t header[8192], *setup, *kernel, *initrd_data;
	int setup_size, initrd_size = 0, cmdline_size;
	size_t kernel_size, read_size;
	/* The xen way */
	struct setup_header *hdr;
	uint32_t hdr_int;
	uint16_t protocol;

	if (argc != 2)
		goto err_out;

	kernel_filename = argv[1];
	ret = stat(kernel_filename, &sb);
	if (ret == -1) {
		fprintf(stderr, "Could not stat() file %s\n", kernel_filename);
		goto err_out;
	}

	kernel_size = sb.st_size;

	printf("kernel:\t%s\n", kernel_filename);
	printf("kernel size:\t%lld bytes\n", kernel_size);

	f = fopen(kernel_filename, "rb");
	if (!f) {
		fprintf(stderr, "unable to load kernel %s: %s\n",
				kernel_filename, strerror(errno));
	}

	read_size = fread(header, 1, min(ARRAY_SIZE(header), kernel_size), f);
	if (read_size != min(ARRAY_SIZE(header), kernel_size)) {
		fprintf(stderr, "Invalid size read for %s: %d (%s)\n",
				kernel_filename, read_size, strerror(errno));
		goto err_out;
	}

	fprintf(stdout, "Going to parse kernel...\n\n");

	memcpy(&hdr_int, HDR_MAGIC, sizeof(uint32_t));

	hdr = (struct setup_header *) header;

	/* Xen's check is by far the cleanest and easiest to read */
	if (memcmp(&hdr->header, HDR_MAGIC, HDR_MAGIC_SZ) != 0 ) {
		fprintf(stderr, "Bad image magic\n");
		fprintf(stdout, "Xen Expects:\t0x%08x\n", hdr_int);
		fprintf(stderr, "On image:\t%0x\n", hdr->header);
		goto err_out;
	}

	fprintf(stdout, "-------------------------------------------------\n");
	if (hdr->setup_sects > 15) {
		char kver_str[128];

		fseek(f, hdr->kernel_version + 0x200, SEEK_SET);
		fread(kver_str, 128, 1, f);
		kver_str[127] = '\0';
		fprintf(stdout, "Kernel version: %s\n", kver_str);
	} else
		fprintf(stdout, "No kernel version information available\n");

	fprintf(stdout, "-------------------------------------------------\n");

	fprintf(stdout, "Xen Expects:\t0x%08x\n", hdr_int);
	fprintf(stdout, "Qemu Expects:\t0x%08x\n", 0x53726448);
	fprintf(stdout, "On image:\t0x%08x\n", hdr->header);

	fprintf(stdout, "\n\n");

	/*
	 * Qemu calls this protocol, on Xen and Linux this is the
	 * boot protocol version. Xen requires at least 2.08.
	 */
	memcpy(&protocol, header+0x206, sizeof(uint16_t));
        fprintf(stdout, "bzImage protocol Version: v%d.%02d\n",
		hdr->version >> 8, hdr->version & 0xff);
    	fprintf(stdout, "Xen hdr->version:\t%d\n", hdr->version);
    	fprintf(stdout, "Qemu protocol:\t\t%d\n", protocol);
    	fprintf(stdout, "Qemu VERSION(2,8):\t%d\n", VERSION(2,8));

    	fprintf(stdout, "\n-------------------------------------------------\n");
    	fprintf(stdout, "Boot protocol 2.07:\t0x%04x\t(supports hardware_subarch)\n", VERSION(2,7));
    	fprintf(stdout, "Boot protocol 2.08:\t0x%04x\n", VERSION(2,8));
    	fprintf(stdout, "Boot protocol 2.09:\t0x%04x\n", VERSION(2,9));
    	fprintf(stdout, "Boot protocol 2.10:\t0x%04x\n", VERSION(2,10));
    	fprintf(stdout, "Boot protocol 2.11:\t0x%04x\n", VERSION(2,11));
    	fprintf(stdout, "Boot protocol 2.12:\t0x%04x\n", VERSION(2,12));
    	fprintf(stdout, "Boot protocol 2.13:\t0x%04x\n", VERSION(2,13));

	/*
	 * Refer to:
	 *
	 * 	Documentation/x86/zero-page.txt
	 * 	arch/x86/include/uapi/asm/bootparam.h
	 *
	 * Upon boot we also use the sruct setup_header on the
	 * struct boot_params. On x86 32-bit this is on the first
	 * page, aka "zero page", on 64-bit this can be anywhere.
	 * Either way we know the sruct setup_header offset within
	 * struct boot_parmams resides between [0x1f1 - 0x290]. Qemu
	 * uses direct offsets from the struct boot_parmams as with:
	 *
	 *    header[0x211] |= 0x80;  // CAN_USE_HEAP
	 *
	 * If we want to modify qemu to add other fields we need to
	 * know the offset. This program skips struct boot_params and
	 * by placing setup_header at 0x1f1 with a pad. To get the
	 * offset of fields we can simply use offsetof. To test
	 * correctness we know qemu's code relies on an offset of
	 * 0x211 for setup_header->loadflags. Test for that and
	 * then compute the offset for hardware_subarch.
	 */

	/* Boot protocol >= 2.07 supports hardware_subarch */

    	fprintf(stdout, "\n\n");
    	fprintf(stdout, "Member\t\t\t\t\tOffset\tExpected\tMatch\n");
    	fprintf(stdout, "-------------------------------------------------------------------------\n");
    	fprintf(stdout, "setup_header->loadflags\t\t\t0x%04x\t0x0211\t\t%s\n",
			offsetof(struct setup_header, loadflags),
			offsetof(struct setup_header, loadflags) == 0x0211 ?
			"YES" : "NO!");
    	fprintf(stdout, "setup_header->hardware_subarch\t\t0x%04x\n",
			offsetof(struct setup_header, hardware_subarch));
    	fprintf(stdout, "setup_header->hardware_subarch_data\t0x%04x\n",
			offsetof(struct setup_header, hardware_subarch_data));

	exit(EXIT_SUCCESS);
err_out:
	usage(argv);
	return ret;
}
