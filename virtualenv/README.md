# Using Pÿthon virtual environments on Magi

Python virtual environment (virtualenv or venv) creates lightweight "virtual environments", each with their own independent set of Python packages installed in their site directories. A virtual environment is created on top of an existing Python installation, known as the virtual environment's "base" Python, and may optionally be isolated from the packages in the base environment, so only those explicitly installed in the virtual environment are available.

You can create a virtualenv this way:

```console
nicolas.greneche@frontend:~$ virtualenv test-venv
created virtual environment CPython3.11.2.final.0-64 in 6329ms
  creator CPython3Posix(dest=/nfs/homes/nicolas.greneche/test-venv, clear=False, no_vcs_ignore=False, global=False)
  seeder FromAppData(download=False, pip=bundle, setuptools=bundle, wheel=bundle, via=copy, app_data_dir=/nfs/homes/nicolas.greneche/.local/share/virtualenv)
    added seed packages: pip==23.0.1, setuptools==66.1.1, wheel==0.38.4
  activators BashActivator,CShellActivator,FishActivator,NushellActivator,PowerShellActivator,PythonActivator
```

Activate it:

```console
nicolas.greneche@frontend:~$ source test-venv/bin/activate
(test-venv) nicolas.greneche@frontend:~$
```

You can notice the prompt alteration. It means that you are in your virtualenv. You can eventually upgrade pip:

```console
(test-venv) nicolas.greneche@frontend:~$ pip install --upgrade pip
Requirement already satisfied: pip in ./test-venv/lib/python3.11/site-packages (23.0.1)
Collecting pip
  Using cached pip-24.3.1-py3-none-any.whl (1.8 MB)
Installing collected packages: pip
  Attempting uninstall: pip
    Found existing installation: pip 23.0.1
    Uninstalling pip-23.0.1:
      Successfully uninstalled pip-23.0.1
Successfully installed pip-24.3.1
```

And install all needed packages (eg numpy):

```console
(test-venv) nicolas.greneche@frontend:~$ pip install numpy
Collecting numpy
  Downloading numpy-2.2.0-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl.metadata (62 kB)
Downloading numpy-2.2.0-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl (16.4 MB)
   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 16.4/16.4 MB 40.4 MB/s eta 0:00:00
Installing collected packages: numpy
Successfully installed numpy-2.2.0
```

You can integrate your virtualenv in submission scripts like this:

```console
#!/bin/bash

#SBATCH --job-name=venv-test
#SBATCH --output=venv-test.out
#SBATCH --error=venv-test.err
#SBATCH --mail-type=end
#SBATCH --partition=SMP-128c

source <path to your virtualenv>/bin/activate
srun python <my python code>
```

