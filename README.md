This is a template project for the STM32F4DISCOVERY board using PlatformIO,
with some built-in helper functions to have printf() output over SWO and for
measuring cycle counts. The clock is initialized at 24 MHz and the ART
accelerator is disabled to achieve deterministic execution times.

It heavily borrows from https://github.com/maxgerhardt/pio-swo-demo,
including its custom SWO viewer target to monitor output directly from
within Visual Studio Code. One can also use `st-trace` from
https://github.com/stlink-org/stlink instead by running the following
command:

`st-trace -c24`

A custom test runner reading data over SWO allows for the use of PlatformIO's
built-in unit testing facilities. Example usage is provided in the tests/
directory.

# Stack usage monitoring

Two facilities are provided to monitor maximum stack usage for the program.

The first uses a sampling-based technique: every 1 ms (assuming the default
SysTick configuration), the current value of the stack pointer is sampled, and
the maximum is updated if necessary. This version does not require
initialization and runs automatically. Whenever the user wants to read a
sample, they can use the macro `stack_usage_sample_get_max()` to get the
largest stack usage sampled until now. `stack_usage_sample_get_last()` is also
provided to read the most recent sample.

The second uses a watermarking-based technique. It must be initialized at the
start of `main()` by calling `stack_usage_scan_init(x)`, where `x` is the
maximum expected stack usage in bytes -- this technique is unable to detect
stack usages beyond the value provided. Whenever the user wants to read the
current value of the stack high watermark (i.e. the largest stack size seen
throughout the execution of the program), they can call the function
`stack_usage_scan()`.

Each technique has advantages and disadvantages. The sampling-based technique
may miss fast (< 1 ms) functions that use a lot of stack, but do not happen
to be running when the sample is taken. The watermarking-based technique may
report incorrect values if the same magic constant used by it is written to
the stack by the program.

Examples are shown for both facilities in the `main()` function of this
template.

# Another tool to analyze code size and stack usage: puncover

Although the stack usage monitoring facilities above help understand global
stack usage, by design it is impossible to drill down into individual
functions to locate the worst offenders. A tool that is able to report
per-function stack usage is puncover (https://github.com/HBehrens/puncover/).
It requires Python and can be installed with the following command:

```
pip install puncover
```

To use the tool, run the following command from the root directory of the
project (i.e. the directory containing the `platformio.ini` file):

```
puncover \
    --gcc_tools_base ~/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi- \
    --elf-file .pio/build/disco_f407vg/firmware.elf \
    --build_dir .pio/build \
    --src_root .
```

This creates a local web server, and opens it in your system's default web
browser, presenting a graphical user interface for code size and stack usage
analysis.

For all the reporting features of this tool to work, it requires that the
binary is compiled with certain flags: `-fstack-usage -fcallgraph-info -g`.
These are already included in the template project (in the `platformio.ini`
file).

However, note that some users have reported inconsistencies and errors in
puncover's output, so keep this in mind, and if possible confirm the reported
values by adding up stack usage by variables and arrays in the target function.

# Cycle count measurement

Macros are provided to aid with benchmarking, using the internal cycle counter
of Cortex-M cores.

To use them, one must first add `bm_decls;` to the same scope as the code to
be benchmarked. This declares variables required by the benchmarking facility.

The code to be benchmarked must be preceded with `bm_start()`, and followed by
`bm_end()`. To get the most recent cycle count, use the `bm_result()` macro.

For long-running functions (i.e. > 1 ms), interrupts such as SysTick will run
during the benchmark, adding noise to the benchmark results. For best results,
disable IRQs prior to calling `bm_start()` using `__disable_irq()`, and
reenable them after `bm_end()` using `__enable_irq()`. Obviously, this may
break code that depends on interrupts.

Example code is shown in the `main()` function of this template.
