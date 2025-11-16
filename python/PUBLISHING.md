# Publishing OmNomNum to PyPI

This guide explains how to publish the OmNomNum Python wrapper to PyPI (Python Package Index).

## Prerequisites

1. PyPI account: https://pypi.org/account/register/
2. TestPyPI account (for testing): https://test.pypi.org/account/register/
3. API tokens for both accounts

## Setup

### 1. Install publishing tools

```bash
pip install build twine
```

### 2. Configure PyPI credentials

Create `~/.pypirc`:

```ini
[distutils]
index-servers =
    pypi
    testpypi

[pypi]
username = __token__
password = pypi-your-api-token-here

[testpypi]
repository = https://test.pypi.org/legacy/
username = __token__
password = pypi-your-testpypi-token-here
```

**Important**: Keep your API tokens secure! Don't commit them to git.

## Publishing Process

### Step 1: Update Version

Update version in:
- `setup.py`
- `pyproject.toml`
- `omnomnum/__init__.py`

```python
__version__ = "0.1.0"  # Update this
```

### Step 2: Clean Build Artifacts

```bash
cd python
make clean
# or
rm -rf build/ dist/ *.egg-info
```

### Step 3: Build Package

```bash
cd python
python -m build
```

This creates:
- `dist/omnomnum-0.1.0.tar.gz` (source distribution)
- `dist/omnomnum-0.1.0-*.whl` (wheel, if applicable)

### Step 4: Check Package

```bash
twine check dist/*
```

Fix any errors before proceeding.

### Step 5: Test on TestPyPI (Recommended)

```bash
twine upload --repository testpypi dist/*
```

Then test installation:

```bash
pip install --index-url https://test.pypi.org/simple/ omnomnum
```

### Step 6: Upload to PyPI

```bash
twine upload dist/*
```

### Step 7: Verify

```bash
pip install omnomnum
python -c "from omnomnum import normalize; print(normalize('one two three'))"
```

## Using GitHub Actions (Recommended)

### Automated Publishing on Release

Add to `.github/workflows/publish.yml`:

```yaml
name: Publish to PyPI

on:
  release:
    types: [published]

jobs:
  publish:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3

    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.11'

    - name: Install dependencies
      run: |
        python -m pip install --upgrade pip
        pip install build twine

    - name: Build package
      run: |
        cd python
        python -m build

    - name: Publish to PyPI
      env:
        TWINE_USERNAME: __token__
        TWINE_PASSWORD: ${{ secrets.PYPI_API_TOKEN }}
      run: |
        cd python
        twine upload dist/*
```

Then add your PyPI API token as a GitHub secret named `PYPI_API_TOKEN`.

### Create a Release

1. Tag the version:
```bash
git tag v0.1.0
git push origin v0.1.0
```

2. Create release on GitHub:
   - Go to Releases
   - Click "Draft a new release"
   - Select the tag
   - Write release notes
   - Publish

GitHub Actions will automatically build and publish to PyPI.

## Version Numbering

Follow [Semantic Versioning](https://semver.org/):

- **MAJOR** version (1.0.0): Incompatible API changes
- **MINOR** version (0.1.0): Add functionality (backwards-compatible)
- **PATCH** version (0.0.1): Bug fixes (backwards-compatible)

## Checklist Before Publishing

- [ ] All tests pass
- [ ] Documentation is up to date
- [ ] CHANGELOG updated
- [ ] Version number bumped in all files
- [ ] Package builds successfully
- [ ] Package validated with `twine check`
- [ ] Tested on TestPyPI
- [ ] Tagged in git
- [ ] Release notes written

## Common Issues

### Import Error After Installation

If users see import errors, ensure `cffi` is properly listed in `install_requires`.

### Build Fails on Windows

Windows users may need Visual Studio Build Tools. Add instructions to README.

### Platform-Specific Wheels

To build wheels for multiple platforms, use `cibuildwheel` in CI:

```bash
pip install cibuildwheel
cibuildwheel --platform linux
```

## Updating Package

To publish an update:

1. Make changes
2. Update version number
3. Update CHANGELOG
4. Follow publishing steps above

## Resources

- PyPI: https://pypi.org
- TestPyPI: https://test.pypi.org
- Packaging Guide: https://packaging.python.org
- CFFI Documentation: https://cffi.readthedocs.io
