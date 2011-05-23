#pragma once
#include "../geometry/rect2d.hpp"
#include "../std/string.hpp"

namespace search
{

// Search result. Search returns a list of them, ordered by score.
class Result
{
public:
  Result(string const & str, m2::RectD const & rect, int penalty);

  string GetString() const { return m_str; }
  m2::RectD GetRect() const { return m_rect; }
  int GetPenalty() const { return m_penalty; }

private:
  string m_str;
  m2::RectD m_rect;
  int m_penalty;
};

}
