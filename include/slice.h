#pragma once

//struct VectorSpriteSlice
//{
//    size_t width{};
//    size_t height{};
//};

struct ArraySizes
{
    size_t rows;
    size_t cols;
};

struct RealAndSliceRowCol
{
    ArraySizes real {};
    ArraySizes slice {};
};
