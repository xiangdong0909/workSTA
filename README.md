# writing STA

## lib
struct Cell { \
  type, \
  {pin1, pin2, time1}, \
  {pin3, pin4, time2}, \
  ... \
};

## netlists
map {cell_name, type}

struct Pin { \
    cell_name, \
    pin_name \
};

map1 {Pin, id}
map2 {id, Pin}

struct Edge { \
    fromPin_id, \
    toPin_id, \
    len \
}

## BuildGraph

dijkstra -> AT
dfs -> RAT
slack = RAT - AT

## Founctions
Founction 1: getMinSlack()
save all begin point slack, get min (similar to singleton pattern)

Founction 2: getEndSlack()
inverse graph?
