.PHONY: install build test clean dev lint format help

help:
	@echo "OmNomNum Python Wrapper - Available targets:"
	@echo ""
	@echo "  install     Install package"
	@echo "  dev         Install package in development mode with dev dependencies"
	@echo "  build       Build package"
	@echo "  test        Run tests"
	@echo "  lint        Run type checking with mypy"
	@echo "  format      Format code with black"
	@echo "  clean       Remove build artifacts"
	@echo "  example     Run example script"
	@echo "  help        Show this help message"

install:
	pip install .

dev:
	pip install -e ".[dev]"

build:
	python setup.py build_ext --inplace

test:
	pytest tests/ -v

lint:
	mypy omnomnum/

format:
	black omnomnum/ tests/ examples/

clean:
	rm -rf build/ dist/ *.egg-info
	rm -rf omnomnum/__pycache__ tests/__pycache__ examples/__pycache__
	rm -rf omnomnum/_omnomnum*.so
	rm -rf .pytest_cache .mypy_cache
	find . -name "*.pyc" -delete
	find . -name "*.pyo" -delete
	find . -name "__pycache__" -type d -exec rm -rf {} + 2>/dev/null || true

example:
	python examples/basic_usage.py
