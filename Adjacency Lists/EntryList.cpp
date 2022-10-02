#include "EntryList.h"

using std::cout;
using std::endl;
using std::range_error;

// Constructor - DO NOT MODIFY
EntryList::EntryList()
{
  _array = new Entry[DEFAULT_SIZE];
  _capacity = DEFAULT_SIZE;
  _size = 0;
}

EntryList::EntryList(const EntryList &rhs)
{

  // Entrylist is not empty
  if (rhs.size() != 0)
  {

    _array = new Entry[rhs._capacity];

    // deep copies contents of array
    for (int i = 0; i < rhs._size; i++)
    {
      _array[i] = rhs._array[i];
    }

    _capacity = rhs._capacity;
    _size = rhs._size;
  }
  else
  {

    _array = new Entry[DEFAULT_SIZE];
    _capacity = DEFAULT_SIZE;
    _size = 0;
  }
}

const EntryList &EntryList::operator=(const EntryList &rhs)
{

  // checks self assignment
  if (this != &rhs)
  {

    // list is not empty
    if (rhs.size() != 0)
    {

      // if memory is already allocated
      if (this->_array)
        delete[] _array;

      _array = new Entry[rhs._capacity];

      // deep copies contents of array
      for (int i = 0; i < rhs._size; i++)
      {
        _array[i] = rhs._array[i];
      }

      _capacity = rhs._capacity;
      _size = rhs._size;
    }
    else
    {

      // deletes array
      if (this->_array)
        delete[] _array;

      _array = new Entry[DEFAULT_SIZE];
      _capacity = DEFAULT_SIZE;
      _size = 0;
    }
  }

  return *this;
}

EntryList::~EntryList()
{

  // deletes array
  if (this->_array)
    delete[] _array;
}

bool EntryList::insert(const Entry &e)
{

  for (int i = 0; i < _size; i++)
  {

    // checks to see if vertex already exists
    if (_array[i].getVertex() == e.getVertex())
    {
      return false;
    }
  }

  // if the array is full
  if (_size == _capacity)
  {

    // array twice the size
    Entry *newArray = new Entry[_capacity * 2];

    for (int i = 0; i < _size; i++)
    {

      // copies contents of array
      newArray[i] = _array[i];
    }

    // deletes and reallocates to array
    delete[] _array;
    _array = newArray;
    _capacity *= 2;
  }

  // adds entry to end of list
  _array[_size] = e;
  _size++;

  EntryList::Entry temp;

  // sorting last inserted entry
  for (int i = _size - 1; i > 0; i--)
  {

    // if index needs to be sorted
    if (_array[i].getVertex() < _array[i - 1].getVertex())
    {

      temp = _array[i];
      _array[i] = _array[i - 1];
      _array[i - 1] = temp;
    }
  }

  return true;
}

bool EntryList::update(const Entry &e)
{

  for (int i = 0; i < _size; i++)
  {

    // finds vertex and updates weight
    if (_array[i].getVertex() == e.getVertex())
    {
      _array[i].setWeight(e.getWeight());

      return true;
    }
  }

  return false;
}

bool EntryList::getEntry(int vertex, Entry &ret)
{

  for (int i = 0; i < _size; i++)
  {

    // if vertex is found updates ret
    if (_array[i].getVertex() == vertex)
    {
      ret = _array[i];

      return true;
    }
  }
  return false;
}

bool EntryList::remove(int vertex, Entry &ret)
{

  for (int i = 0; i < _size; i++)
  {

    // if vertex is found updates ret
    if (_array[i].getVertex() == vertex)
    {
      ret = _array[i];

      // shifts over elements of array
      for (int j = i; j < _size; j++)
      {
        _array[j] = _array[j + 1];
      }

      _size--;

      // checks to resize array
      if (_size < (_capacity / 4))
      {

        // new array half the normal size
        Entry *newArray = new Entry[_capacity / 2];

        for (int i = 0; i < _size; i++)
        {

          // copies contents of array
          newArray[i] = _array[i];
        }

        // deletes and reallocates to array
        delete[] _array;
        _array = newArray;
        _capacity /= 2;
      }

      return true;
    }
  }

  return false;
}

EntryList::Entry &EntryList::at(int indx) const
{

  if (indx < 0 || indx >= _size)
  {

    throw range_error("Out Of Range");
  }
  return _array[indx];
}

// dump data structure - DO NOT MODIFY
void EntryList::dump()
{
  for (int i = 0; i < _size; i++)
  {
    cout << "  " << _array[i] << endl;
  }
}

EntryList::Iterator::Iterator(EntryList *EList, int indx)
{

  _ELptr = EList;
  _indx = indx;
}

bool EntryList::Iterator::operator!=(const EntryList::Iterator &rhs)
{

  // rhs doesnt equal this iterator
  return (_indx != rhs._indx) || (_ELptr != rhs._ELptr);
}

bool EntryList::Iterator::operator==(const EntryList::Iterator &rhs)
{

  // rhs equals this iterator
  return (_indx == rhs._indx) && (_ELptr == rhs._ELptr);
}

void EntryList::Iterator::operator++(int dummy)
{

  // index is not above size
  if (_indx < _ELptr->size())
    _indx++;
}

EntryList::Entry EntryList::Iterator::operator*()
{

  return _ELptr->at(_indx);
}

EntryList::Iterator EntryList::begin()
{

  Iterator it(this, 0);
  return it;
}

EntryList::Iterator EntryList::end()
{

  Iterator it(this, _size);
  return it;
}

// Insertion operator for Entry objects - DO NOT MODIFY
ostream &operator<<(ostream &sout, const EntryList::Entry &e)
{
  sout << e._vertex << ": " << e._weight;
  return sout;
}

// A convenient way to write test code for a single class is to write
// a main() function at the bottom of the .cpp file.  Just be sure to
// comment-out main() once you are done testing!

// Following is example test code.  There is no guarantee that it is
// complete -- you are responsbile for thoroughly testing your code.
// In particular, passing these tests does not mean your
// implementation will pass all grading tests.
/*
int main()
{
  EntryList *e1 = new EntryList();
  EntryList *e2 = new EntryList();
  EntryList::Entry e;

  cout << "size: " << e1->size() << ", capacity: " << e1->capacity() << endl;
  e1->dump();
  cout << endl;

  for (int i = 1; i < 13; i++)
  {
    e1->insert(EntryList::Entry((i * 5) % 13, i));
  }

  cout << "size: " << e1->size() << ", capacity: " << e1->capacity() << endl;
  e1->dump();
  cout << endl;

  *e2 = *e1;

  for (int i = 1; i < 13; i += 2)
  {
    e2->remove(i, e);
  }

  cout << "size: " << e1->size() << ", capacity: " << e1->capacity() << endl;
  e1->dump();
  cout << endl;

  cout << "size: " << e2->size() << ", capacity: " << e2->capacity() << endl;
  e2->dump();
  cout << endl;

  for (int i = 2; i < 13; i+=2) {
    el.update( EntryList::Entry(i, 2*i) );
  }

  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;

  for (int i = 3; i < 13; i*=2) {
    el.remove(i, e);
  }

  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;

cout << "\nPrint using iterator:\n";
for (auto itr = el.begin(); itr != el.end(); itr++) {
  cout << *itr << endl;
}
  return 0;
}*/