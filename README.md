# Building and Installation Guide (WSL)

This project is a hybrid C++/Python network simulator. It consists of a core C++ static library (`wikipedia_core`), a standalone C++ debug executable (`main`), and a Python extension module (`_core`) powered by **pybind11**.

> ⚠️ **Note for Windows Users:** This guide assumes you are building and running the project inside **WSL (Windows Subsystem for Linux)**. All commands must be executed within your WSL terminal. The resulting Python module will be a Linux shared library (`.so`) and must be executed using the Python interpreter inside WSL.

---

## 🛠️ Prerequisites

Before building the project, ensure your WSL environment has the required toolchains and libraries installed.

### Installing Dependencies

Run the following commands inside your WSL terminal (assumes Ubuntu/Debian distribution):

```bash
sudo apt update
sudo apt install build-essential cmake python3-dev libspdlog-dev libfmt-dev
pip install pybind11