#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;

// Constructor - DO NOT MODIFY
Graph::Graph(int n)
{
  if (n <= 0)
    throw invalid_argument("Graph::Graph(): number of vertices must be positive");

  _rows = new EntryList *[n];
  for (int i = 0; i < n; i++)
  {
    _rows[i] = new EntryList();
  }
  _numVert = n;
  _numEdge = 0;
}

Graph::Graph(const Graph &G)
{

  // G is not empty
  if (G._numVert != 0)
  {

    _numVert = G._numVert;
    _numEdge = G._numEdge;

    // allocates memory to _rows and copies array
    _rows = new EntryList *[G.numVert()];
    for (int i = 0; i < G.numVert(); i++)
    {
      _rows[i] = new EntryList(*G._rows[i]);
    }
  }
}

const Graph &Graph::operator=(const Graph &rhs)
{

  // checks self assignment
  if (this != &rhs)
  {

    // if graph is not empty
    if (rhs._numVert != 0)
    {

      // if memory is already allocated
      if (this->_rows)
      {

        // deallocates _rows
        for (int i = 0; i < _numVert; i++)
        {
          delete _rows[i];
        }

        delete[] _rows;
      }

      _numVert = rhs._numVert;
      _numEdge = rhs._numEdge;

      // reallocates _rows and copies array
      _rows = new EntryList *[rhs.numVert()];
      for (int i = 0; i < rhs.numVert(); i++)
      {
        _rows[i] = new EntryList(*rhs._rows[i]);
      }
    }
  }

  return *this;
}

Graph::~Graph()
{

  // removes new entrylist()
  for (int i = 0; i < _numVert; i++)
  {
    delete _rows[i];
  }

  // deallocates _rows
  delete[] _rows;
}

// Number of vertices - DO NOT MODIFY
int Graph::numVert() const
{
  return _numVert;
}

// Number of edges - DO NOT MODIFY
int Graph::numEdge() const
{
  return _numEdge;
}

void Graph::addEdge(int u, int v, weight_t x)
{

  // if vertex is out of bounds
  if (u < 0 || u >= numVert())
  {
    throw invalid_argument("Add edge is out of bounds");
  }

  EntryList::Entry newEntry(v, x);

  // tries to insert
  if (_rows[u]->insert(newEntry))
    _numEdge++;
}

bool Graph::removeEdge(int u, int v)
{

  // if vertex is out of bounds
  if (u < 0 || u >= numVert())
  {
    throw invalid_argument("Remove edge is out of bounds");
  }

  EntryList::Entry removedEntry;

  // tries to remove
  if (_rows[u]->remove(v, removedEntry))
  {
    _numEdge--;
    return true;
  }

  return false;
}

// Dump the graph - DO NOT MODIFY
void Graph::dump() const
{

  cout << "Dump of graph (numVert = " << _numVert
       << ", numEdge = " << _numEdge << ")" << endl;

  for (int i = 0; i < _numVert; i++)
  {
    cout << "Row " << i << ": \n";
    _rows[i]->dump();
  }
}

Graph::EgIterator::EgIterator(Graph *Gptr, bool enditr)
{

  // Gptr is null
  if (Gptr == nullptr)
  {

    // indicates an uninitalized varible
    _Gptr = nullptr;
  }
  else
  {

    _Gptr = Gptr;

    // last iterator
    if (enditr)
    {
      // finds the last edge
      for (int i = Gptr->numVert() - 1; i >= 0; i--)
      {

        if (Gptr->_rows[i]->size() > 0)
        {

          _row = i;
          _itr = Gptr->_rows[i]->end();
          break;
        }
      }
    }

    // beginning iterator
    else
    {
      // finds the first edge
      for (int i = 0; i < Gptr->numVert(); i++)
      {

        if (Gptr->_rows[i]->size() > 0)
        {

          _row = i;
          _itr = Gptr->_rows[i]->begin();
          break;
        }
      }
    }
  }
}

tuple<int, int, weight_t> Graph::EgIterator::operator*()
{

  if (_Gptr == nullptr)
    throw invalid_argument("Uninitialized iterator");

  tuple<int, int, weight_t> edge;

  // makes tuple
  edge = std::make_tuple(_row, (*_itr).getVertex(), (*_itr).getWeight());

  return edge;
}

bool Graph::EgIterator::operator!=(const EgIterator &rhs)
{

  // not the same row, iterator, and G pointer
  return ((_row != rhs._row) || (_itr != rhs._itr)) || (_Gptr != rhs._Gptr);
}

void Graph::EgIterator::operator++(int dummy)
{

  if (_Gptr == nullptr)
    throw invalid_argument("EgIterator uninitialized iterator");

  EntryList::Iterator endit;

  for (int i = _Gptr->numVert() - 1; i >= 0; i--)
  {

    if (_Gptr->_rows[i]->size() > 0)
    {

      endit = _Gptr->_rows[i]->end();

      // break loop
      i = -1;
    }
  }

  // checks if iterator is already at the end
  if (_itr != endit)
  {

    // next edge
    _itr++;

    do
    {

      // if itr is at the end of row
      if (_itr == _Gptr->_rows[_row]->end() && _itr != endit)
      {

        _row++;
        _itr = _Gptr->_rows[_row]->begin();
      }

      // while entrylist has no entries or row is out of bounds
    } while ((_Gptr->_rows[_row]->size() <= 0) && _itr != endit);
  }
}

Graph::EgIterator Graph::egBegin()
{

  EgIterator it(this, false);
  return it;
}

Graph::EgIterator Graph::egEnd()
{

  EgIterator it(this, true);
  return it;
}

Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool enditr)
{

  // Gptr is null
  if (Gptr == nullptr)
  {
    _Gptr = nullptr;
  }
  else
  {
    _Gptr = Gptr;

    if (v < 0 || v >= _Gptr->_numVert)
      throw invalid_argument("NbIterator invalid vertex");

    // last iterator
    if (enditr)
    {

      _row = v;
      _itr = Gptr->_rows[v]->end();
    }
    // beginning iterator
    else
    {
      _row = v;
      _itr = Gptr->_rows[v]->begin();
    }
  }
}

bool Graph::NbIterator::operator!=(const NbIterator &rhs)
{

  // not the same row, iterator, and G pointer
  return (_row != rhs._row) || (_itr != rhs._itr) || (_Gptr != rhs._Gptr);
}

void Graph::NbIterator::operator++(int dummy)
{

  // if itr is not at the end
  if (_itr != _Gptr->_rows[_row]->end())
    _itr++;
}

pair<int, weight_t> Graph::NbIterator::operator*()
{

  if (_Gptr == nullptr)
    throw invalid_argument("NBIterator uninitialized iterator");

  pair<int, weight_t> neigh;

  // creates pair
  neigh.first = (*_itr).getVertex();
  neigh.second = (*_itr).getWeight();

  return neigh;
}

Graph::NbIterator Graph::nbBegin(int v)
{

  NbIterator it(this, v, false);
  return it;
}

Graph::NbIterator Graph::nbEnd(int v)
{

  NbIterator it(this, v, true);
  return it;
}
