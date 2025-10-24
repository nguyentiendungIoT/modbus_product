.PHONY: help install install-dev test coverage lint format clean build publish

help:
	@echo "Available commands:"
	@echo "  make install       - Install package"
	@echo "  make install-dev   - Install package with dev dependencies"
	@echo "  make test          - Run tests"
	@echo "  make coverage      - Run tests with coverage report"
	@echo "  make lint          - Run linters (flake8, mypy)"
	@echo "  make format        - Format code with black and isort"
	@echo "  make clean         - Remove build artifacts"
	@echo "  make build         - Build distribution packages"
	@echo "  make publish       - Publish to PyPI"

install:
	pip install -e .

install-dev:
	pip install -r requirements-dev.txt
	pip install -e .

test:
	pytest

coverage:
	pytest --cov=modbus_product --cov-report=html --cov-report=term
	@echo "Coverage report generated in htmlcov/index.html"

lint:
	flake8 modbus_product tests --max-line-length=88 --extend-ignore=E203,W503
	mypy modbus_product --ignore-missing-imports

format:
	black modbus_product tests examples
	isort modbus_product tests examples

clean:
	rm -rf build/
	rm -rf dist/
	rm -rf *.egg-info
	rm -rf .pytest_cache
	rm -rf .coverage
	rm -rf htmlcov/
	rm -rf .mypy_cache
	find . -type d -name __pycache__ -exec rm -rf {} +
	find . -type f -name '*.pyc' -delete

build: clean
	python -m build

publish: build
	twine upload dist/*

.DEFAULT_GOAL := help
