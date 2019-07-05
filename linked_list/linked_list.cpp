#include <iostream>
#include <string>

using namespace std;

template<class val_type>
class Single_linked_node
{
  public:

  Single_linked_node() : val_{}, next_{nullptr}
  {}

  Single_linked_node(val_type val) : val_{val}, next_{nullptr}
  {}

  Single_linked_node(const Single_linked_node&) = delete;
  Single_linked_node(Single_linked_node&&) = delete;

  Single_linked_node& operator=(const Single_linked_node&) = delete;
  Single_linked_node& operator=(Single_linked_node&&) = delete;

  virtual ~Single_linked_node()
  {
    cout << this << " single list node (val = " << val_ << ", next = " << next_ << ") destr\n";

    val_ = {};
    next_ = { nullptr };
  }

  val_type Get_val()
  {
    return val_;
  }

  Single_linked_node* Get_next()
  {
    return next_;
  }

  template<class val_type> friend class Single_linked_list;

  private:

  val_type val_;
  Single_linked_node *next_;
};

template<class val_type>
class Single_linked_list
{
  public:
    
  Single_linked_list() : first_{nullptr}, size_{}
  {}

  virtual ~Single_linked_list()
  {
    while (nullptr != first_)
    {
      void *tmp { first_->next_ };

      delete first_;

      first_ = { static_cast<Single_linked_node<val_type>*>(tmp) };

      size_--;
    }

    if (0 != size_)
    {
      cerr << "~Single_linked_list: all elements were destroyed, but size = " << size_ << " (" << __FILE__ << ", " << __LINE__ << ")\n";
    }

    cout << this << " single linked list destr\n";
  }

  Single_linked_node<val_type>* Get_first()
  {
    return first_;
  }

  bool Emplace_element(val_type val, Single_linked_node<val_type> *after, Single_linked_node<val_type> **result)
  {
    if (nullptr == result)
    {
      cerr << "Emplace_element: Result node wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { new Single_linked_node<val_type>(val) };

      *result = { first_ };

      size_++;
      return true;
    }

    // Add as a first node
    if (nullptr == after)
    {
      *result = { new Single_linked_node<val_type>(val) };

      if (nullptr == *result)
      {
        cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";
        return false;
      }

      Single_linked_node<val_type> *tmp { first_ };

      first_ = { *result };

      (*result)->next_ = { tmp };

      size_++;

      return true;
    }

    if (false == Find_node(after))
    {
      cerr << "Emplace_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    *result = { new Single_linked_node<val_type>(val) };

    if (nullptr == *result)
    {
      cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    Single_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { *result };

    (*result)->next_ = { tmp };

    size_++;

    return true;
  }

  bool Add_element(Single_linked_node<val_type> **el, Single_linked_node<val_type> *after)
  {
    if (nullptr == el || nullptr == *el)
    {
      cerr << "Add_element: Element to add wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    if (nullptr != after && false == Find_node(after))
    {
      cerr << "Add_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { *el };
      size_++;

      return true;
    }

    // Add as a first node
    if (nullptr == after)
    {
      Single_linked_node<val_type> *tmp { first_ };

      first_ = { *el };

      (*el)->next_ = { tmp };

      size_++;

      return true;
    }

    Single_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { *el };

    (*el)->next_ = { tmp };

    size_++;

    return true;
  }

  bool Find_parent(Single_linked_node<val_type> *el, Single_linked_node<val_type> **parent)
  {
    if (nullptr == el)
    {
      cerr << "Find_parent: Node to find wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == first_)
    {
      cerr << "Find_parent: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == parent)
    {
      cerr << "Find_parent: Result node wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    Single_linked_node<val_type> *curr { first_ };

    while (nullptr != curr)
    {
      if (curr->next_ == el)
      {
        *parent = { curr };
        return true;
      }

      curr = { curr->next_ };
    }

    *parent = { nullptr };

    return false;
  }

  bool Delete_element(Single_linked_node<val_type> *el)
  {
    if (nullptr == el)
    {
      cerr << "Delete_element: Node to delete wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (false == Find_node(el))
    {
      cerr << "Delete_element: Failed to find node " << el << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (el == first_)
    {
      first_ = { el->next_ };
      el->~Single_linked_node();
      size_--;

      return true;
    }

    Single_linked_node<val_type> *parent { new Single_linked_node<val_type> };

    if (false == Find_parent(el, &parent))
    {
      cerr << "Delete_element: node " << el << " isn't the first node, but doesn't have parents (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }
    else if (nullptr == parent)
    {
      cerr << "Delete_element: parent, provided by Find_parent, is nullptr: " << parent << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    if (nullptr == el->next_)
    {
      // No need to connect the el's child with a new parent

      parent->next_ = { nullptr };
      el->~Single_linked_node();
      size_--;

      return true;
    }
    else
    {
      // el's child will need a new parent -- el's parent

      parent->next_ = { el->next_ };
      el->~Single_linked_node();
      size_--;

      return true;
    }

    // Unreacheable for now.
    //cout << "Delete_element: Failed to delete node " << el << " from single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    //return false;
  }

  bool Find_value_first_occur(val_type val, Single_linked_node<val_type> **result)
  {
    if (nullptr == result || nullptr == *result)
    {
      cerr << "Find_value_first_occur: Result node wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == first_)
    {
      cerr << "Find_value_first_occur: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    Single_linked_node<val_type> *curr { first_ };

    while (true)
    {
      if (curr->val_ == val)
      {
        *result = { curr };
        return true;
      }

      curr = curr->next_;
    }

    *result = { nullptr };
    cout << "Find_value_first_occur: Failed to find node vith val " << val << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    return false;
  }

  bool Find_node(Single_linked_node<val_type> *node)
  {
    if (nullptr == node)
    {
      cerr << "Find_node: Node to find wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == first_)
    {
      cerr << "Find_node: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    Single_linked_node<val_type> *curr { first_ };

    while (nullptr != curr)
    {
      if (curr == node)
      {
        return true;
      }

      curr = { curr->next_ };
    }

    cout << "Find_node: Node " << node << " is not a part of linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    return false;
  }

  Single_linked_node<val_type>* operator[](size_t index) const
  {
    if (nullptr == first_)
    {
      cerr << "Single_linked_node operator[]: no elements (" << __FILE__ << ", " << __LINE__ << ")\n";
      return nullptr;
    }

    if (index >= size_)
    {
      cerr << "Single_linked_node operator[]: index " << index << " is out of bounds (" << __FILE__ << ", " << __LINE__ << ")\n";
      return nullptr;
    }

    Single_linked_node<val_type> *curr{ first_ };
    val_type ret{};

    for (size_t i{}; i < index; ++i)
    {
      if (nullptr != curr)
      {
        curr = { curr->next_ };
      }
      else
      {
        cerr << "Single_linked_node operator[]: Can't access element with index" << index << " (" << __FILE__ << ", " << __LINE__ << ")\n";
        return nullptr;
      }
    }

    return curr;
  }

  size_t Get_size() const noexcept
  {
    return size_;
  }

  bool Move_after(size_t what, size_t after)
  {
    if (what >= size_)
    {
      cerr << "Single_linked_node Move_after: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (after >= size_)
    {
      cerr << "Single_linked_node Move_after: after (" << after << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == after)
    {
      return true;
    }

    if (abs(what - after) == 1 && what > after)
    {
      return true;
    }

    Single_linked_node<val_type> *what_ptr{ operator[](what) };
    Single_linked_node<val_type> *after_ptr{ operator[](after) };

    return Move(what_ptr, after_ptr);
  }

  bool Move_before(size_t what, size_t before)
  {
    if (what >= size_)
    {
      cerr << "Single_linked_node Move_before: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (before >= size_)
    {
      cerr << "Single_linked_node Move_before: before (" << before << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == before)
    {
      return true;
    }

    if (abs(what - before) == 1 && what < before)
    {
      return true;
    }

    Single_linked_node<val_type> *what_ptr{ operator[](what) };

    if (before > 0)
    {
      Single_linked_node<val_type> *after_ptr{ operator[](before - 1) };

      return Move(what_ptr, after_ptr);
    }
    else
    {
      return Move(what_ptr, nullptr);
    }
  }

  bool Swap_vals(size_t a, size_t b)
  {
    if (a >= size_)
    {
      cerr << "Single_linked_node Swap: a (" << a << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (b >= size_)
    {
      cerr << "Single_linked_node Swap: b (" << b << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    Single_linked_node<val_type> *a_ptr{ operator[](a) };
    Single_linked_node<val_type> *b_ptr{ operator[](b) };

    return Swap_vals(a_ptr, b_ptr);
  }

  bool Swap_vals(Single_linked_node<val_type>* a, Single_linked_node<val_type>* b)
  {
    if (nullptr == a)
    {
      cerr << "Single_linked_node Swap: a (" << a << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (nullptr == b)
    {
      cerr << "Single_linked_node Swap: b (" << b << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    if (false == Find_node(a))
    {
      cerr << "Single_linked_node Swap: node a (" << a << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(b))
    {
      cerr << "Single_linked_node Swap: node b (" << b << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    val_type tmp{ a->val_ };
    a->val_ = { b->val_ };
    b->val_ = { tmp };

    return true;
  }

  private:

  bool Move(Single_linked_node<val_type>* what, Single_linked_node<val_type>* after)
  {
    if (nullptr == what)
    {
      cerr << "Single_linked_node Move: what (" << what << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(what))
    {
      cerr << "Single_linked_node Move: node what (" << what << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == after)
    {
      return true;
    }

    Single_linked_node<val_type> *what_prev_old { /*what->prev_*/ };
    Find_parent(what, &what_prev_old);

    Single_linked_node<val_type> *what_next_old{ what->next_ };
    Single_linked_node<val_type> *after_next_old{};

    if (nullptr != after)
    {
      after_next_old = { after->next_ };
      after->next_ = { nullptr };
    }

    /*what->prev_ =*/ what->next_ = { nullptr };

    if (nullptr != what_prev_old)
    {
      what_prev_old->next_ = { what_next_old };
    }

    /*if (nullptr != what_next_old)
    {
      what_next_old->prev_ = { what_prev_old };
    }*/

    if (nullptr != after)
    {
      after->next_ = { what };
    }

    //what->prev_ = { after };

    if (nullptr != after_next_old)
    {
      what->next_ = { after_next_old };
    }
    else if (nullptr == after)
    {
      what->next_ = { first_ };
    }

    /*if (nullptr != after_next_old)
    {
      after_next_old->prev_ = { what };
    }*/

    if (what == first_)
    {
      first_ = { what_next_old };
    }

    if (nullptr == after)
    {
      //first_->prev_ = { what };
      first_ = { what };
    }

    return true;
  }
  
  std::size_t size_;
  Single_linked_node<val_type> *first_;
};

template<class val_type>
class Double_linked_node
{
  public:

  Double_linked_node() : val_{}, prev_{ nullptr }, next_{ nullptr }
  {}

  Double_linked_node(val_type val) : val_{ val }, prev_{ nullptr }, next_{ nullptr }
  {}

  Double_linked_node(const Double_linked_node<val_type>&) = delete;
  Double_linked_node(Double_linked_node<val_type>&&) = delete;

  Double_linked_node& operator=(const Double_linked_node<val_type>&) = delete;
  Double_linked_node& operator=(Double_linked_node<val_type>&&) = delete;

  virtual ~Double_linked_node()
  {
    cout << this << " double linked node (val = " << val_ << ", prev = " << prev_ << ", next = " << next_ << ") destr\n";

    val_  = {};
    next_ = { nullptr };
    prev_ = { nullptr };
  }

  val_type Get_val()
  {
    return val_;
  }

  Double_linked_node<val_type>* Get_prev()
  {
    return prev_;
  }

  Double_linked_node<val_type>* Get_next()
  {
    return next_;
  }

  template<class val_type> friend class Double_linked_list;

  private:

  val_type val_;

  Double_linked_node<val_type> *prev_;
  Double_linked_node<val_type> *next_;
};

template<class val_type>
class Double_linked_list
{
  public:

  Double_linked_list() : first_{ nullptr }, size_{}
  {}

  virtual ~Double_linked_list()
  {
    while (nullptr != first_)
    {
      void *tmp { first_->next_ };

      delete first_;

      first_ = { static_cast<Double_linked_node<val_type>*>(tmp) };

      size_--;
    }

    if (0 != size_)
    {
      cerr << "~Double_linked_list: all elements were destroyed, but size = " << size_ << " (" << __FILE__ << ", " << __LINE__ << ")\n";
    }

    cout << this << " double linked list destr\n";
  }

  Double_linked_node<val_type>* Get_first()
  {
    return first_;
  }

  bool Emplace_element(val_type val, Double_linked_node<val_type> *after, Double_linked_node<val_type> **result)
  {
    if (nullptr == result)
    {
      cerr << "Emplace_element: Result node wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { new Double_linked_node<val_type>(val) };

      *result = { first_ };
      size_++;
      return true;
    }

    // Add as a first node
    if (nullptr == after)
    {
      *result = { new Double_linked_node<val_type>(val) };

      if (nullptr == *result)
      {
        cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";
        return false;
      }

      Double_linked_node<val_type> *tmp{ first_ };

      first_ = { *result };

      (*result)->next_ = { tmp };
      tmp->prev_ = (*result);

      size_++;

      return true;
    }

    if (false == Find_node(after))
    {
      cerr << "Emplace_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    *result = { new Double_linked_node<val_type>(val) };

    if (nullptr == *result)
    {
      cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    Double_linked_node<val_type> *tmp{ after->next_ };

    after->next_ = { *result };

    (*result)->next_ = { tmp };
    (*result)->prev_ = { after };

    if (nullptr != tmp)
    {
      tmp->prev_ = { *result };
    }
    size_++;

    return true;
  }

  bool Add_element(Double_linked_node<val_type> **el, Double_linked_node<val_type> *after)
  {
    if (nullptr == el || nullptr == *el)
    {
      cerr << "Add_element: Element to add wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    if (nullptr != after && false == Find_node(after))
    {
      cerr << "Add_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { *el };
      size_++;

      return true;
    }

    // Add as a first node
    if (nullptr == after)
    {
      Double_linked_node<val_type> *tmp{ first_ };

      first_ = { *el };

      (*el)->next_ = { tmp };
      tmp->prev_ = (*el);

      size_++;

      return true;
    }

    Double_linked_node<val_type> *tmp{ after->next_ };

    after->next_ = { *el };

    (*el)->next_ = { tmp };
    (*el)->prev_ = { after };

    if (nullptr != tmp)
    {
      tmp->prev_ = { *el };
    }
    size_++;

    return true;
  }

  bool Delete_element(Double_linked_node<val_type> *el)
  {
    if (nullptr == el)
    {
      cerr << "Delete_element: Node to delete wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (false == Find_node(el))
    {
      cerr << "Delete_element: Failed to find node " << el << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (el == first_)
    {
      first_ = { el->next_ };
      el->~Double_linked_node();
      size_--;

      return true;
    }

    Double_linked_node<val_type> *parent { el->prev_ };

    if (nullptr == parent)
    {
      cerr << "Delete_element: node " << el << " isn't the first node, but doesn't have parents (" << __FILE__ << ' ' << __LINE__ << ")\n";
      return false;
    }

    if (nullptr == el->next_)
    {
      // No need to connect the el's child with a new parent

      parent->next_ = { nullptr };
      el->~Double_linked_node();
      size_--;

      return true;
    }
    else
    {
      // el's child will need a new parent -- el's parent

      parent->next_ = { el->next_ };
      el->~Double_linked_node();
      size_--;

      return true;
    }

    // Unreacheable for now
    //cout << "Delete_element: Failed to delete node " << el << " from single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    //return false;
  }

  bool Find_value_first_occur(val_type val, Double_linked_node<val_type> **result)
  {
    if (nullptr == result || nullptr == *result)
    {
      cerr << "Find_value_first_occur: Result node wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == first_)
    {
      cerr << "Find_value_first_occur: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    Double_linked_node<val_type> *curr{ first_ };

    while (nullptr != curr)
    {
      if (curr->val_ == val)
      {
        *result = { curr };
        return true;
      }

      curr = curr->next_;
    }

    *result = { nullptr };
    cout << "Find_value_first_occur: Failed to find node vith val " << val << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    return false;
  }

  bool Find_node(Double_linked_node<val_type> *node)
  {
    if (nullptr == node)
    {
      cerr << "Find_node: Node to find wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (nullptr == first_)
    {
      cerr << "Find_node: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    Double_linked_node<val_type> *curr{ first_ };

    while (nullptr != curr)
    {
      if (curr == node)
      {
        return true;
      }

      curr = { curr->next_ };
    }

    cout << "Find_node: Node " << node << " is not a part of linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    return false;
  }

  Double_linked_node<val_type>* operator[](size_t index) const
  {
    if (nullptr == first_)
    {
      cerr << "Double_linked_node operator[]: no elements (" << __FILE__ << ", " << __LINE__ << ")\n";
      return nullptr;
    }

    if (index >= size_)
    {
      cerr << "Double_linked_node operator[]: index " << index << " is out of bounds (" << __FILE__ << ", " << __LINE__ << ")\n";
      return nullptr;
    }

    Double_linked_node<val_type> *curr{ first_ };
    val_type ret{};

    for (size_t i{}; i < index; ++i)
    {
      if (nullptr != curr)
      {
        curr = { curr->next_ };
      }
      else
      {
        cerr << "Double_linked_node operator[]: Can't access element with index" << index << " (" << __FILE__ << ", " << __LINE__ << ")\n";
        return nullptr;
      }
    }

    return curr;
  }

  size_t Get_size() const noexcept
  {
    return size_;
  }

  bool Move_after(size_t what, size_t after)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_after: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (after >= size_)
    {
      cerr << "Double_linked_node Move_after: after (" << after << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == after)
    {
      return true;
    }

    if (abs(what - after) == 1 && what > after)
    {
      return true;
    }

    Double_linked_node<val_type> *what_ptr  { operator[](what) };
    Double_linked_node<val_type> *after_ptr { operator[](after) };

    return Move(what_ptr, after_ptr);
  }

  bool Move_before(size_t what, size_t before)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_before: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (before >= size_)
    {
      cerr << "Double_linked_node Move_before: before (" << before << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == before)
    {
      return true;
    }

    if (abs(what - before) == 1 && what < before)
    {
      return true;
    }

    Double_linked_node<val_type> *what_ptr{ operator[](what) };

    if (before > 0)
    {
      Double_linked_node<val_type> *after_ptr{ operator[](before - 1) };

      return Move(what_ptr, after_ptr);
    }
    else
    {
      return Move(what_ptr, nullptr);
    }
  }

  bool Swap_vals(size_t a, size_t b)
  {
    if (a >= size_)
    {
      cerr << "Double_linked_node Swap: a (" << a << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (b >= size_)
    {
      cerr << "Double_linked_node Swap: b (" << b << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    Double_linked_node<val_type> *a_ptr { operator[](a) };
    Double_linked_node<val_type> *b_ptr { operator[](b) };

    return Swap_vals(a_ptr, b_ptr);
  }

  bool Swap_vals(Double_linked_node<val_type>* a, Double_linked_node<val_type>* b)
  {
    if (nullptr == a)
    {
      cerr << "Double_linked_node Swap: a (" << a << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (nullptr == b)
    {
      cerr << "Double_linked_node Swap: b (" << b << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    if (false == Find_node(a))
    {
      cerr << "Double_linked_node Swap: node a (" << a << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(b))
    {
      cerr << "Double_linked_node Swap: node b (" << b << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    val_type tmp{ a->val_ };
    a->val_ = { b->val_ };
    b->val_ = { tmp };

    return true;
  }

  private:

  bool Move(Double_linked_node<val_type>* what, Double_linked_node<val_type>* after)
  {
    if (nullptr == what)
    {
      cerr << "Double_linked_node Move: what (" << what << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(what))
    {
      cerr << "Double_linked_node Move: node what (" << what << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (what == after)
    {
      return true;
    }

    Double_linked_node<val_type> *what_prev_old{ what->prev_ };
    Double_linked_node<val_type> *what_next_old{ what->next_ };
    Double_linked_node<val_type> *after_next_old{};

    if (nullptr != after)
    {
      after_next_old = { after->next_ };
      after->next_ = { nullptr };
    }

    what->prev_ = what->next_ = { nullptr };

    if (nullptr != what_prev_old)
    {
      what_prev_old->next_ = { what_next_old };
    }

    if (nullptr != what_next_old)
    {
      what_next_old->prev_ = { what_prev_old };
    }

    if (nullptr != after)
    {
      after->next_ = { what };
    }

    what->prev_ = { after };
    if (nullptr != after_next_old)
    {
      what->next_ = { after_next_old };
    }
    else if (nullptr == after)
    {
      what->next_ = { first_ };
    }

    if (nullptr != after_next_old)
    {
      after_next_old->prev_ = { what };
    }

    if (what == first_)
    {
      first_ = { what_next_old };
    }

    if (nullptr == after)
    {
      first_->prev_ = { what };
      first_ = { what };
    }

    return true;
  }

  size_t size_;
  Double_linked_node<val_type> *first_;
};

int main()
{
  do
  {
    cout << "Single\n";

    Single_linked_list<double> sl;

    Single_linked_node<double> *node_1   { new Single_linked_node<double>(1.77) };

    Single_linked_node<double> *emplaced { new Single_linked_node<double> };

    if (false == sl.Emplace_element(3.14, nullptr, &emplaced))
    {
      return -1;
    }

    if (false == sl.Emplace_element(2.0, emplaced, &emplaced))
    {
      return -1;
    }

    if (false == sl.Emplace_element(3.0, emplaced, &emplaced))
    {
      return -1;
    }

    Single_linked_node<double> *four { new Single_linked_node<double> };

    if (false == sl.Emplace_element(4.0, emplaced, &four))
    {
      return -1;
    }
  
    if (false == sl.Emplace_element(5.0, four, &emplaced))
    {
      return -1;
    }

    if (false == sl.Emplace_element(4.5, four, &emplaced))
    {
      return -1;
    }

    if (false == sl.Add_element(&node_1, emplaced))
    {
      return -1;
    }

    Single_linked_node<double> *curr { sl.Get_first() };

    while (nullptr != curr)
    {
      cout << curr->Get_val() << ' ';
      curr = curr->Get_next();
    }

    cout << endl;

    cout << "size " << sl.Get_size() << endl;

    Single_linked_node<double> *parent { new Single_linked_node<double> };

    if (sl.Find_parent(emplaced, &parent) && nullptr != parent)
    {
      cout << "Parent of " << emplaced->Get_val() << ": " << parent->Get_val();
    }

    cout << endl;

    cout << "Move 5 after 6: " << sl.Move_after(5, 6) << endl;

    for (size_t i{}; i < sl.Get_size(); ++i)
    {
      cout << sl[i]->Get_val() << ' ';
    }

    cout << endl;

    Single_linked_node<double> *found { new Single_linked_node<double> };

    if (sl.Find_value_first_occur(5.0, &found) && nullptr != found)
    {
      sl.Delete_element(found);
    }

    cout << "size " << sl.Get_size() << endl;

    curr = sl.Get_first();

    while (nullptr != curr)
    {
      cout << curr->Get_val() << ' ';
      curr = curr->Get_next();
    }

    cout << endl;

    for (size_t i{}; i < sl.Get_size(); ++i)
    {
      cout << sl[i]->Get_val() << ' ';
    }

    cout << endl;

    /////////////////////////////////////////////////////////////////////////////////////

    cout << "Double:\n";

    Double_linked_list<double> dl;

    Double_linked_node<double> *node_2     { new Double_linked_node<double>(1.77) };

    Double_linked_node<double> *emplaced_2 { new Double_linked_node<double> };

    if (false == dl.Emplace_element(3.14, nullptr, &emplaced_2))
    {
      return -1;
    }

    if (false == dl.Emplace_element(2.0, emplaced_2, &emplaced_2))
    {
      return -1;
    }

    if (false == dl.Emplace_element(3.0, emplaced_2, &emplaced_2))
    {
      return -1;
    }

    Double_linked_node<double> *four_2 { new Double_linked_node<double> };

    if (false == dl.Emplace_element(4.0, emplaced_2, &four_2))
    {
      return -1;
    }

    if (false == dl.Emplace_element(5.0, four_2, &emplaced_2))
    {
      return -1;
    }

    if (false == dl.Emplace_element(4.5, four_2, &emplaced_2))
    {
      return -1;
    }

    if (false == dl.Add_element(&node_2, emplaced_2))
    {
      return -1;
    }

    Double_linked_node<double> *curr_2 { dl.Get_first() };

    while (nullptr != curr_2)
    {
      cout << curr_2->Get_val() << ' ';
      curr_2 = { curr_2->Get_next() };
    }

    cout << endl;

    cout << "size " << dl.Get_size() << endl;

    Double_linked_node<double> *parent_2 { emplaced_2->Get_prev() };

    if (nullptr != parent_2)
    {
      cout << "Parent of " << emplaced->Get_val() << ": " << emplaced_2->Get_prev()->Get_val();
    }

    cout << endl;

    cout << "Move 6 after 0: " << dl.Move_after(6, 0) << endl;

    //dl.Swap_vals(emplaced_2, emplaced_2->Get_prev());

    for (size_t i{}; i < dl.Get_size(); ++i)
    {
      cout << dl[i]->Get_val() << ' ';
    }

    cout << endl;

    Double_linked_node<double> *found_2 { new Double_linked_node<double> };

    if (dl.Find_value_first_occur(4.5, &found_2) && nullptr != found_2)
    {
      dl.Delete_element(found_2);
    }

    cout << "size " << dl.Get_size() << endl;

    curr_2 = dl.Get_first();

    while (nullptr != curr_2)
    {
      cout << curr_2->Get_val() << ' ';
      curr_2 = { curr_2->Get_next() };
    }

    cout << endl;

    for (size_t i {}; i < dl.Get_size(); ++i)
    {
      cout << dl[i]->Get_val() << ' ';
    }

    cout << endl;
  }
  while (false);

  cin.get();

  return 0;
}
