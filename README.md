<div align="center">

<pre>
██████╗ ██╗   ██╗ ██████╗██╗  ██╗ ██████╗ ███████╗
██╔══██╗██║   ██║██╔════╝██║ ██╔╝██╔═══██╗██╔════╝
██║  ██║██║   ██║██║     █████╔╝ ██║   ██║███████╗
██║  ██║██║   ██║██║     ██╔═██╗ ██║   ██║╚════██║
██████╔╝╚██████╔╝╚██████╗██║  ██╗╚██████╔╝███████║
╚═════╝  ╚═════╝  ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
</pre>

### A tiny educational x86 operating system, written from scratch in C and Assembly

</div>

DuckOS is a small monolithic kernel for x86, plus a usable shell with a handful of builtins, built to be **readable end-to-end** rather than feature-complete. The whole thing fits in a few thousand lines so the boot path, descriptor tables, paging, console driver and keyboard handler can all be studied without getting lost.

## Features

- **Multiboot-compliant boot sector** — boots under GRUB on real x86 or QEMU.
- **Protected mode**: GDT + IDT, interrupt handlers, traps.
- **Memory management** — segmentation + paging (`mm/page.asm`, `mm/memory.c`).
- **VGA text console** with colors.
- **PS/2 keyboard driver** with scancode → ASCII translation.
- **TTY / serial output**.
- **Cooperative scheduler skeleton**.
- **Interactive shell** with builtins:
  `art`, `cat`, `cd`, `color`, `echo`, `fs`, `ls`, `mkdir`, `pwd`, `quit`, `rm`, `touch`, `vim`, `write`.
- **ASCII art splash** loaded from `ART/DuckOS.txt`.

## Architecture

```
┌──────────────────────────────────────────────────────────┐
│                        GRUB (multiboot)                  │
└────────────────────────────┬─────────────────────────────┘
                             ▼
                    ┌────────────────┐
                    │   boot/boot.asm│   set up stack, jump to C
                    └────────┬───────┘
                             ▼
                    ┌────────────────┐
                    │   init/main.c  │   kernel entry
                    └────────┬───────┘
            ┌────────────────┼─────────────────┐
            ▼                ▼                 ▼
      ┌──────────┐    ┌────────────┐    ┌────────────┐
      │  kernel/ │    │     mm/    │    │   lib/     │
      │ GDT/IDT  │    │ paging +   │    │ libc bits  │
      │ traps    │    │ segments   │    │ (string…)  │
      │ keyboard │    └────────────┘    └────────────┘
      │ console  │
      │ tty/sched│
      └────┬─────┘
           ▼
   ┌─────────────────┐
   │  tools/shell.c  │  ───▶  features/*.c  (builtins)
   └─────────────────┘
```

## Build

Requirements: an x86 host, `nasm`, `gcc`, GNU `ld`, GRUB, optionally `qemu-system-i386`.

```bash
make            # produces duckos.bin + system.map
make run        # boots duckos.bin under QEMU
make clean
```

## Project layout

```
duckos/
├── boot/        # bootloader (NASM) + link.ld
├── init/        # kernel entry (main.c)
├── kernel/      # console, keyboard, TTY, descriptors, traps, sched, …
├── mm/          # paging + memory.c
├── fs/          # filesystem stubs
├── lib/         # tiny libc (string.c)
├── include/
│   ├── duckos/  # kernel-internal headers
│   ├── asm/     # arch-specific
│   └── sys/     # syscall / cdefs
├── tools/
│   └── shell.c  # interactive shell
├── features/    # shell builtins (art, cat, cd, color, echo, fs,
│                #                 ls, mkdir, pwd, quit, rm, touch,
│                #                 vim, write)
├── ART/         # splash + ASCII art (DuckOS.txt, ascii.txt)
├── Makefile
├── AUTHORS
└── LICENSE      # GPL-3.0
```

## Roadmap

- Real filesystem (currently `fs/` is stubs).
- Userspace + syscalls.
- Preemptive scheduling.
- Floppy / ATA driver.

## Authors

Fernando Bezerra ([@Nandobez](https://github.com/Nandobez)).

## License

Released under the **GNU General Public License v3.0** — see [`LICENSE`](./LICENSE).
