# Catch2 tests and code coverage

![Report](coverage_report/GCC%20Code%20Coverage%20Report.png)

## How to build App-Test binary
- There is a separate `CMakeLists.txt` file for testing purposes.
- Go into `/test_bin` folder and type:
  ```txt
  cmake ..
  make
  ```
- Run the `App-Test`.

## 1. Tests implemented with Catch2.
- The App's main is replaced with Catch2's main.
- This lets us simulate user input because Catch2 obviously does not have a mouse and keyboard to use when testing.

## 2. Code coverage implemented with gcovr
- A Python tool that produces visual reports of gcov.
- Type `gcovr -r ../../src . --html --html-details --html-details -o report.html` in the terminal.
- Open up the newly generated `report.html` in your internet browser.
- Or see the PDF reports in the `coverage_report` folder.
- How to get gcovr:
    ```txt
    python -m pip install gcovr
    or
    python3 -m pip install gcovr
    ```
