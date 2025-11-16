"""
Tests for OmNomNum Python wrapper.
"""

import pytest
from omnomnum import normalize, find_numbers, OmNomNum


class TestNormalize:
    """Tests for normalize function."""

    def test_simple_numbers(self):
        """Test simple number normalization."""
        assert normalize("one") == "1"
        assert normalize("two") == "2"
        assert normalize("twenty three") == "23"
        assert normalize("one hundred") == "100"
        assert normalize("two hundred fifty") == "250"

    def test_large_numbers(self):
        """Test large number normalization."""
        assert normalize("one thousand") == "1000"
        assert normalize("one million") == "1000000"
        assert normalize("two thousand twenty three") == "2023"

    def test_fractions(self):
        """Test fraction normalization."""
        assert normalize("one half") == "0.5"
        assert normalize("three quarters") == "0.75"
        assert normalize("one and a half") == "1.5"

    def test_decimals(self):
        """Test decimal normalization."""
        assert normalize("one point five") == "1.5"
        assert normalize("three point one four") == "3.14"

    def test_negatives(self):
        """Test negative numbers."""
        assert normalize("negative five") == "-5"
        assert normalize("minus ten") == "-10"

    def test_ordinals(self):
        """Test ordinal numbers."""
        assert normalize("first") == "1st"
        assert normalize("second") == "2nd"
        assert normalize("third") == "3rd"
        assert normalize("twenty third") == "23rd"

    def test_mixed_content(self):
        """Test text with mixed numbers and words."""
        result = normalize("I have two cats")
        assert "2" in result
        assert "cats" in result

    def test_no_numbers(self):
        """Test text without numbers."""
        text = "hello world"
        assert normalize(text) == text

    def test_precision(self):
        """Test precision control."""
        # Default precision (6)
        result = normalize("one third")
        assert result.startswith("0.333333")

        # Custom precision
        result = normalize("one third", precision=2)
        assert result == "0.33"


class TestFindNumbers:
    """Tests for find_numbers function."""

    def test_find_single_number(self):
        """Test finding a single number."""
        numbers = find_numbers("I have two cats")
        assert len(numbers) == 1
        assert numbers[0].value == 2.0

    def test_find_multiple_numbers(self):
        """Test finding multiple numbers."""
        numbers = find_numbers("two cats and three dogs")
        assert len(numbers) == 2
        assert numbers[0].value == 2.0
        assert numbers[1].value == 3.0

    def test_find_no_numbers(self):
        """Test finding no numbers."""
        numbers = find_numbers("hello world")
        assert len(numbers) == 0

    def test_number_positions(self):
        """Test that number positions are correct."""
        text = "I have two cats"
        numbers = find_numbers(text)
        assert len(numbers) == 1

        num = numbers[0]
        word = text[num.begin : num.end]
        assert word == "two"


class TestOmNomNumClass:
    """Tests for OmNomNum class."""

    def test_default_constructor(self):
        """Test default constructor."""
        nom = OmNomNum()
        assert nom.normalize("one") == "1"

    def test_parse_fractions_option(self):
        """Test parse_fractions option."""
        # With fraction parsing (default)
        nom = OmNomNum(parse_fractions=True)
        assert nom.normalize("one half") == "0.5"

        # Without fraction parsing
        nom = OmNomNum(parse_fractions=False)
        assert nom.normalize("one half") == "one half"

    def test_precision_option(self):
        """Test precision option."""
        nom = OmNomNum(precision=2)
        result = nom.normalize("one third")
        assert result == "0.33"

    def test_reuse_instance(self):
        """Test reusing the same instance."""
        nom = OmNomNum()
        assert nom.normalize("one") == "1"
        assert nom.normalize("two") == "2"
        assert nom.normalize("three") == "3"

    def test_set_precision(self):
        """Test changing precision dynamically."""
        nom = OmNomNum(precision=6)
        result1 = nom.normalize("one third")
        assert result1.startswith("0.333333")

        nom.set_precision(2)
        result2 = nom.normalize("one third")
        assert result2 == "0.33"

    def test_set_parse_fractions(self):
        """Test changing parse_fractions dynamically."""
        nom = OmNomNum(parse_fractions=True)
        assert nom.normalize("one half") == "0.5"

        nom.set_parse_fractions(False)
        assert nom.normalize("one half") == "one half"


class TestNumberClass:
    """Tests for Number class."""

    def test_number_attributes(self):
        """Test Number object attributes."""
        numbers = find_numbers("two")
        assert len(numbers) == 1

        num = numbers[0]
        assert num.is_double is True
        assert num.value == 2.0
        assert num.begin >= 0
        assert num.end > num.begin

    def test_number_repr(self):
        """Test Number string representation."""
        numbers = find_numbers("two")
        assert len(numbers) == 1
        assert "Number" in repr(numbers[0])
        assert "value=2.0" in repr(numbers[0])

    def test_number_to_dict(self):
        """Test Number to_dict method."""
        numbers = find_numbers("two")
        assert len(numbers) == 1

        d = numbers[0].to_dict()
        assert "value" in d
        assert d["value"] == 2.0
        assert "begin" in d
        assert "end" in d


class TestEdgeCases:
    """Tests for edge cases."""

    def test_empty_string(self):
        """Test empty string."""
        assert normalize("") == ""

    def test_whitespace_only(self):
        """Test whitespace-only string."""
        result = normalize("   ")
        # Should preserve whitespace
        assert result.strip() == ""

    def test_unicode_text(self):
        """Test unicode text."""
        # Should handle non-ASCII text around numbers
        result = normalize("café has two tables")
        assert "2" in result

    def test_type_error(self):
        """Test type checking."""
        nom = OmNomNum()
        with pytest.raises(TypeError):
            nom.normalize(123)  # Not a string

        with pytest.raises(TypeError):
            nom.find_numbers(None)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
