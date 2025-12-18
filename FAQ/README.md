# Errors FAQ

## Setting up ulimit

If you experience messages like:

```bash
[1765653660.705652] [magi175:640743:0] ib_md.c:287 UCX ERROR ibv_reg_mr(address=0x15213b956010, length=65536, access=0xf) failed: Cannot allocate memory : Please set max locked memory (ulimit -l) to 'unlimited' (current: 524288 kbytes)
```

You can prepend the srun directive in your submission script with **ulimit -l unlimited**:

```bash
nicolas.greneche@frontend:~$ cat my_submission_script.run
...

ulimit -l unlimited
srun my_program

...
```
