#pragma once

#include "../base/base.hpp"
#include "../base/buffer_vector.hpp"
#include "../std/queue.hpp"

namespace search
{

typedef uint32_t UniChar;

namespace impl
{

struct MatchCostData
{
  uint32_t m_A, m_B;
  uint32_t m_Cost;

  MatchCostData() : m_A(0), m_B(0), m_Cost(0) {}
  MatchCostData(uint32_t a, uint32_t b, uint32_t cost) : m_A(a), m_B(b), m_Cost(cost) {}

  bool operator < (MatchCostData const & o) const
  {
    return m_Cost > o.m_Cost;
  }
};

template <typename PriorityQueyeT>
void PushMatchCost(PriorityQueyeT & q, uint32_t maxCost, uint32_t a, uint32_t b, uint32_t cost)
{
  if (cost <= maxCost)
    q.push(MatchCostData(a, b, cost));
}

}  // namespace search::impl

class DefaultMatchCost
{
public:
  uint32_t Cost10(UniChar a) const;
  uint32_t Cost01(UniChar b) const;
  uint32_t Cost11(UniChar a, UniChar b) const;
  uint32_t Cost12(UniChar a, UniChar const * pB) const;
  uint32_t Cost21(UniChar const * pA, UniChar b) const;
  uint32_t Cost22(UniChar const * pA, UniChar const * pB) const;
  uint32_t SwapCost(UniChar a1, UniChar a2) const;
};

template <typename CharT, typename CostF>
uint32_t StringMatchCost(CharT const * sA, uint32_t sizeA,
                         CharT const * sB, uint32_t sizeB,
                         CostF const & costF, uint32_t maxCost,
                         bool bPrefixMatch = false)
{
  priority_queue<impl::MatchCostData, buffer_vector<impl::MatchCostData, 256> > q;
  q.push(impl::MatchCostData(0, 0, 0));
  while (!q.empty())
  {
    uint32_t a = q.top().m_A;
    uint32_t b = q.top().m_B;
    uint32_t const c = q.top().m_Cost;
    q.pop();
    while (a < sizeA && b < sizeB && sA[a] == sB[b])
      ++a, ++b;

    if (a == sizeA && (bPrefixMatch || b == sizeB))
      return c;

    if (a < sizeA)
      impl::PushMatchCost(q, maxCost, a + 1, b, c + costF.Cost10(sA[a]));
    if (b < sizeB)
      impl::PushMatchCost(q, maxCost, a, b + 1, c + costF.Cost01(sB[b]));
    if (a < sizeA && b < sizeB)
      impl::PushMatchCost(q, maxCost, a + 1, b + 1, c + costF.Cost11(sA[a], sB[b]));
    if (a + 1 < sizeA && b < sizeB)
      impl::PushMatchCost(q, maxCost, a + 2, b + 1, c + costF.Cost21(&sA[a], sB[b]));
    if (a < sizeA && b + 1 < sizeB)
      impl::PushMatchCost(q, maxCost, a + 1, b + 2, c + costF.Cost12(sA[a], &sB[b]));
    if (a + 1 < sizeA && b + 1 < sizeB)
    {
      impl::PushMatchCost(q, maxCost, a + 2, b + 2, c + costF.Cost22(&sA[a], &sB[b]));
      if (sA[a] == sB[b + 1] && sA[a + 1] == sB[b])
        impl::PushMatchCost(q, maxCost, a + 2, b + 2, c + costF.SwapCost(sA[a], sA[a + 1]));
    }
  }
  return maxCost + 1;
}

}  // namespace search
