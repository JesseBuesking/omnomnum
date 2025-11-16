# Contributing to OmNomNum Python Wrapper

Thank you for your interest in contributing! This document provides guidelines for contributing to the OmNomNum Python wrapper.

## Development Setup

1. Clone the repository:
```bash
git clone https://github.com/JesseBuesking/omnomnum.git
cd omnomnum/python
```

2. Install in development mode:
```bash
make dev
# or
pip install -e ".[dev]"
```

3. Verify installation:
```bash
python -c "from omnomnum import normalize; print(normalize('one two three'))"
```

## Development Workflow

### Running Tests

```bash
make test
# or
pytest tests/ -v
```

### Code Formatting

We use [Black](https://github.com/psf/black) for code formatting:

```bash
make format
# or
black omnomnum/ tests/ examples/
```

### Type Checking

We use [mypy](https://mypy.readthedocs.io/) for type checking:

```bash
make lint
# or
mypy omnomnum/
```

### Running Examples

```bash
make example
# or
python examples/basic_usage.py
```

## Project Structure

```
python/
├── omnomnum/           # Main package
│   ├── __init__.py     # Package exports
│   ├── wrapper.py      # Python wrapper classes
│   └── build_cffi.py   # CFFI bindings builder
├── tests/              # Test suite
│   └── test_omnomnum.py
├── examples/           # Example scripts
│   └── basic_usage.py
├── setup.py            # Package setup
├── pyproject.toml      # Build configuration
└── README.md           # Documentation
```

## Making Changes

### Adding New Features

1. Create a new branch:
```bash
git checkout -b feature/your-feature-name
```

2. Make your changes to the code

3. Add tests for new functionality in `tests/test_omnomnum.py`

4. Update documentation in `README.md` if needed

5. Run tests and formatting:
```bash
make test
make format
make lint
```

6. Commit your changes:
```bash
git commit -m "Add feature: description"
```

7. Push and create a pull request

### Fixing Bugs

1. Create a new branch:
```bash
git checkout -b fix/bug-description
```

2. Add a test that reproduces the bug

3. Fix the bug

4. Verify the test passes

5. Commit and create a pull request

## Code Style

- Follow PEP 8 guidelines
- Use Black for formatting (line length: 88)
- Add type hints where appropriate
- Write docstrings for public functions and classes
- Keep functions focused and single-purpose

## Writing Tests

- Use pytest for testing
- Aim for high test coverage
- Test edge cases and error conditions
- Use descriptive test names

Example:
```python
def test_normalize_simple_numbers():
    """Test normalization of simple numbers."""
    assert normalize("one") == "1"
    assert normalize("twenty three") == "23"
```

## Documentation

- Update README.md for user-facing changes
- Add docstrings to new functions/classes
- Include examples in docstrings
- Update API reference if needed

## CFFI Bindings

When modifying C bindings in `build_cffi.py`:

1. Keep C definitions in sync with C headers
2. Test on multiple platforms if possible
3. Ensure memory management is correct
4. Add error handling for C function calls

## Pull Request Process

1. Ensure all tests pass
2. Update documentation
3. Add a clear description of changes
4. Reference any related issues
5. Wait for review from maintainers

## Questions?

Feel free to open an issue for:
- Questions about contributing
- Clarification on project structure
- Discussion of potential features

## License

By contributing, you agree that your contributions will be licensed under the BSD 3-Clause License.
