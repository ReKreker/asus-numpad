#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class PadPadding
{
    float top_ = 0, right_ = 0, bottom_ = 0, left_ = 0;
    uint8_t num_rows_ = 0, num_cols_ = 0;

  public:
    explicit PadPadding() = default;
    explicit PadPadding(float top, float right, float bottom, float left)
        : top_(top), right_(right), bottom_(bottom), left_(left)
    {
    }

    void operator()(float top, float right, float bottom, float left)
    {
        top_ = top;
        right_ = right;
        bottom_ = bottom;
        left_ = left;
    }
    void operator()(const std::string &top, const std::string &right, const std::string &bottom,
                    const std::string &left)
    {
        top_ = std::stof(top);
        right_ = std::stof(right);
        bottom_ = std::stof(bottom);
        left_ = std::stof(left);
    }

    bool empty() const
    {
        return !top_ || !right_ || !bottom_ || !left_;
    }

    void set_pad(uint8_t rows_amount, uint8_t cols_amount)
    {
        num_cols_ = cols_amount;
        num_rows_ = rows_amount;
    }

    int32_t get_column(float x) const
    {
        if (left_ < x && x < 1.0 - right_)
            return ((x - left_ - 0.01) * num_cols_) / (1.0 - left_ - right_);
        return -1;
    }
    int32_t get_row(float y) const
    {
        if (top_ < y && y < 1.0 - bottom_)
            return ((y - top_ - 0.01) * num_rows_) / (1.0 - top_ - bottom_);
        return -1;
    }
};

class Layout
{
  public:
    Layout(const std::vector<int> keys, uint8_t num_cols, PadPadding &padding)
        : padding_{padding}, num_cols_{num_cols}, keys_{keys}
    {
        padding_.set_pad(keys_.size() / num_cols_, num_cols_);
    }
    // TODO: add percentage KEY_5 wrapper
    int operator()(float x, float y) const
    {
        const int32_t col = padding_.get_column(x);
        const int32_t row = padding_.get_row(y);
        if (col == -1 or row == -1)
            return 0;
        return keys_[row * num_cols_ + col];
    }

    auto begin() const
    {
        return keys_.begin();
    }
    auto end() const
    {
        return keys_.end();
    }

  private:
    PadPadding padding_;
    const uint8_t num_cols_;
    const std::vector<int> keys_;
};

Layout parse_layout(const std::filesystem::path &path, std::string model = "");