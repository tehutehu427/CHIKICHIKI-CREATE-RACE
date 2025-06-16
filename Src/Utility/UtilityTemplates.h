#pragma once
#include <vector>
#include <algorithm>

// テンプレート専用ユーティリティクラス
class UtilityTemplates
{
public:
    /// <summary>
    /// 配列内に値があるか調べる
    /// </summary>
    /// <typeparam name="T">配列内の要素の型</typeparam>
    /// <param name="vec">検索対象の配列</param>
    /// <param name="value">検索する値</param>
    /// <returns>値が見つかればtrue,なければfalseを返す</returns>
    template<typename T>
    static bool ContainsValue(const std::vector<T>& vec, const T& value) 
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }
};