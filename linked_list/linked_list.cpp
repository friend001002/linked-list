#include <iostream>
#include <string>

using namespace std;

class Single_linked_list_ex : public runtime_error
{
  public:

  explicit Single_linked_list_ex(const char *msg) : runtime_error(msg)
  {
  }

  explicit Single_linked_list_ex(string& msg) : runtime_error(msg)
  {
  }

  virtual ~Single_linked_list_ex()
  {
  }

  virtual const char* what() const noexcept
  {
    return runtime_error::what();
  }
};

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

  Single_linked_node<val_type>* Emplace_element(val_type val, Single_linked_node<val_type> *after) noexcept(false)
  {
    Single_linked_node<val_type> *res { nullptr };

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { new Single_linked_node<val_type>(val) };

      size_++;
      return first_;
    }

    // Add as a first node
    if (nullptr == after)
    {
      res = { new Single_linked_node<val_type>(val) };

      if (nullptr == res)
      {
        cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";
        
        throw Single_linked_list_ex("Failed to create result node\n");
      }

      Single_linked_node<val_type> *tmp { first_ };

      first_ = { res };

      res->next_ = { tmp };

      size_++;

      return res;
    }

    if (false == Find_node(after))
    {
      cerr << "Emplace_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      
      throw Single_linked_list_ex("Failed to find node\n");
    }

    res = { new Single_linked_node<val_type>(val) };

    if (nullptr == res)
    {
      cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Failed to create result node\n");
    }

    Single_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { res };

    res->next_ = { tmp };

    size_++;

    return res;
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

  Single_linked_node<val_type>* Add_element(Single_linked_node<val_type> *el, Single_linked_node<val_type> *after) noexcept(false)
  {
    if (nullptr == el)
    {
      cerr << "Add_element: el must not be nullptr in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("el must not be nullptr");
    }

    if (nullptr != after && false == Find_node(after))
    {
      cerr << "Add_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Failed to find node");
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { el };
      size_++;

      return first_;
    }

    // Add as a first node
    if (nullptr == after)
    {
      Single_linked_node<val_type> *tmp{ first_ };

      first_ = { el };

      el->next_ = { tmp };

      size_++;

      return first_;
    }

    Single_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { el };

    el->next_ = { tmp };

    size_++;

    return after->next_;
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

  Single_linked_node<val_type>* Find_parent(Single_linked_node<val_type> *el) noexcept(false)
  {
    if (nullptr == el)
    {
      cerr << "Find_parent: Node to find wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Node to find wasn't provided");
    }

    if (nullptr == first_)
    {
      cerr << "Find_parent: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Single linked list doesn't have any nodes");
    }

    Single_linked_node<val_type> *curr { first_ };

    while (nullptr != curr)
    {
      if (curr->next_ == el)
      {
        return curr;
      }

      curr = { curr->next_ };
    }

    throw Single_linked_list_ex("Failed to find a parent");
  }

  bool Delete_element_cs(Single_linked_node<val_type> *el)
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

  void Delete_element_ex(Single_linked_node<val_type> *el) noexcept(false)
  {
    if (nullptr == el)
    {
      cerr << "Delete_element: Node to delete wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Node to delete wasn't provided");
    }

    if (false == Find_node(el))
    {
      cerr << "Delete_element: Failed to find node " << el << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Failed to find node in single linked list");
    }

    if (el == first_)
    {
      first_ = { el->next_ };
      el->~Single_linked_node();
      size_--;

      return;
    }

    try
    {
      Single_linked_node<val_type> *parent = Find_parent(el);

      if (nullptr == parent)
      {
        cerr << "Delete_element: parent, provided by Find_parent, is nullptr: " << parent << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

        throw Single_linked_list_ex("Parent, provided by Find_parent, is nullptr");
      }

      if (nullptr == el->next_)
      {
        // No need to connect the el's child with a new parent

        parent->next_ = { nullptr };
        el->~Single_linked_node();
        size_--;

        return;
      }
      else
      {
        // el's child will need a new parent -- el's parent

        parent->next_ = { el->next_ };
        el->~Single_linked_node();
        size_--;

        return;
      }
    }
    catch (Single_linked_list_ex& ex)
    {
      throw Single_linked_list_ex("Provided node isn't the first node, but it doesn't have parents");
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

  Single_linked_node<val_type>* Find_value_first_occur(val_type val) noexcept(false)
  {
    if (nullptr == first_)
    {
      cerr << "Find_value_first_occur: Single linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Single_linked_list_ex("Single linked list doesn't have any nodes");
    }

    Single_linked_node<val_type> *curr { first_ };

    while (true)
    {
      if (curr->val_ == val)
      {
        return curr;
      }

      curr = curr->next_;
    }

    throw Single_linked_list_ex("Failed to find node in single linked list");
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

  Single_linked_node<val_type>* operator[](size_t index) const noexcept(false)
  {
    if (nullptr == first_)
    {
      cerr << "Single_linked_node operator[]: no elements (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Single linked list has no elements");
    }

    if (index >= size_)
    {
      cerr << "Single_linked_node operator[]: index " << index << " is out of bounds (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Single linked list: index is out of bounds");
    }

    Single_linked_node<val_type> *curr { first_ };
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

        throw Single_linked_list_ex("Can't access element with this index");
      }
    }

    return curr;
  }

  size_t Get_size() const noexcept
  {
    return size_;
  }

  bool Move_after_cs(size_t what, size_t after)
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

    return Move_cs(what_ptr, after_ptr);
  }

  void Move_after_ex(size_t what, size_t after) noexcept(false)
  {
    if (what >= size_)
    {
      cerr << "Single_linked_node Move_after: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_after_ex: 'what' must be < size");
    }

    if (after >= size_)
    {
      cerr << "Single_linked_node Move_after: after (" << after << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_after_ex: 'after' must be < size");
    }

    if (what == after)
    {
      return;
    }

    if (abs(what - after) == 1 && what > after)
    {
      return;
    }

    Single_linked_node<val_type> *what_ptr{ operator[](what) };
    Single_linked_node<val_type> *after_ptr{ operator[](after) };

    return Move_ex(what_ptr, after_ptr);
  }

  bool Move_before_cs(size_t what, size_t before)
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

      return Move_cs(what_ptr, after_ptr);
    }
    else
    {
      return Move_cs(what_ptr, nullptr);
    }
  }

  void Move_before_ex(size_t what, size_t before) noexcept(false)
  {
    if (what >= size_)
    {
      cerr << "Single_linked_node Move_before: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_before_ex: 'what' must be < size");
    }

    if (before >= size_)
    {
      cerr << "Single_linked_node Move_before: before (" << before << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_before_ex: 'before' must be < size");
    }

    if (what == before)
    {
      return;
    }

    if (abs(what - before) == 1 && what < before)
    {
      return;
    }

    Single_linked_node<val_type> *what_ptr{ operator[](what) };

    if (before > 0)
    {
      Single_linked_node<val_type> *after_ptr{ operator[](before - 1) };

      return Move_ex(what_ptr, after_ptr);
    }
    else
    {
      return Move_ex(what_ptr, nullptr);
    }
  }

  bool Swap_vals_cs(size_t a, size_t b)
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

    Single_linked_node<val_type> *a_ptr { operator[](a) };
    Single_linked_node<val_type> *b_ptr { operator[](b) };

    return Swap_vals_cs(a_ptr, b_ptr);
  }

  void Swap_vals_ex(size_t a, size_t b) noexcept(false)
  {
    if (a >= size_)
    {
      cerr << "Single_linked_node Swap: a (" << a << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Swap_vals_ex: 'a' must be < size");
    }

    if (b >= size_)
    {
      cerr << "Single_linked_node Swap: b (" << b << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Swap_vals_ex: 'b' must be < size");
    }

    if (a == b)
    {
      return;
    }

    Single_linked_node<val_type> *a_ptr{ operator[](a) };
    Single_linked_node<val_type> *b_ptr{ operator[](b) };

    return Swap_vals_ex(a_ptr, b_ptr);
  }

  bool Swap_vals_cs(Single_linked_node<val_type>* a, Single_linked_node<val_type>* b)
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

  void Swap_vals_ex(Single_linked_node<val_type>* a, Single_linked_node<val_type>* b) noexcept(false)
  {
    if (nullptr == a)
    {
      cerr << "Single_linked_node Swap: a (" << a << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Swap_vals_ex: 'a' must not be nullptr");
    }

    if (nullptr == b)
    {
      cerr << "Single_linked_node Swap: b (" << b << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Single_linked_list_ex("Swap_vals_ex: 'b' must not be nullptr");
    }

    if (a == b)
    {
      return;
    }

    if (false == Find_node(a))
    {
      cerr << "Single_linked_node Swap: node a (" << a << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Swap_vals_ex: 'a' must be a part of linked list");
    }

    if (false == Find_node(b))
    {
      cerr << "Single_linked_node Swap: node b (" << b << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Single_linked_list_ex("Swap_vals_ex: 'b' must be a part of linked list");
    }

    val_type tmp{ a->val_ };
    a->val_ = { b->val_ };
    b->val_ = { tmp };

    return;
  }

  private:

  bool Move_cs(Single_linked_node<val_type>* what, Single_linked_node<val_type>* after)
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

    what->next_ = { nullptr };

    if (nullptr != what_prev_old)
    {
      what_prev_old->next_ = { what_next_old };
    }

    if (nullptr != after)
    {
      after->next_ = { what };
    }

    if (nullptr != after_next_old)
    {
      what->next_ = { after_next_old };
    }
    else if (nullptr == after)
    {
      what->next_ = { first_ };
    }

    if (what == first_)
    {
      first_ = { what_next_old };
    }

    if (nullptr == after)
    {
      first_ = { what };
    }

    return true;
  }

  void Move_ex(Single_linked_node<val_type>* what, Single_linked_node<val_type>* after) noexcept(false)
  {
    if (nullptr == what)
    {
      cerr << "Single_linked_node Move: what (" << what << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_ex: 'what' must not be nullptr");
    }

    if (false == Find_node(what))
    {
      cerr << "Single_linked_node Move: node what (" << what << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";

      throw Single_linked_list_ex("Move_ex: 'what' must be a part of linked list");
    }

    if (what == after)
    {
      return;
    }

    Single_linked_node<val_type> *what_prev_old{ /*what->prev_*/ };
    Find_parent(what, &what_prev_old);

    Single_linked_node<val_type> *what_next_old{ what->next_ };
    Single_linked_node<val_type> *after_next_old{};

    if (nullptr != after)
    {
      after_next_old = { after->next_ };
      after->next_ = { nullptr };
    }

    what->next_ = { nullptr };

    if (nullptr != what_prev_old)
    {
      what_prev_old->next_ = { what_next_old };
    }

    if (nullptr != after)
    {
      after->next_ = { what };
    }

    if (nullptr != after_next_old)
    {
      what->next_ = { after_next_old };
    }
    else if (nullptr == after)
    {
      what->next_ = { first_ };
    }

    if (what == first_)
    {
      first_ = { what_next_old };
    }

    if (nullptr == after)
    {
      first_ = { what };
    }

    return;
  }
  
  std::size_t size_;
  Single_linked_node<val_type> *first_;
};

class Double_linked_list_ex : public runtime_error
{
  public:

  explicit Double_linked_list_ex(const char *msg) : runtime_error(msg)
  {
  }

  explicit Double_linked_list_ex(string& msg) : runtime_error(msg)
  {
  }

  virtual ~Double_linked_list_ex()
  {
  }

  virtual const char* what() const noexcept
  {
    return runtime_error::what();
  }
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

  Double_linked_node<val_type>* Emplace_element(val_type val, Double_linked_node<val_type> *after) noexcept(false)
  {
    Double_linked_node<val_type> *res { nullptr };

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { new Double_linked_node<val_type>(val) };

      size_++;
      return first_;
    }

    // Add as a first node
    if (nullptr == after)
    {
      res = { new Double_linked_node<val_type>(val) };

      if (nullptr == res)
      {
        cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";

        throw Double_linked_list_ex("Failed to create result node\n");
      }

      Double_linked_node<val_type> *tmp{ first_ };

      first_ = { res };

      res->next_ = { tmp };
      tmp->prev_ = res;

      size_++;

      return res;
    }

    if (false == Find_node(after))
    {
      cerr << "Emplace_element: Failed to find node " << after << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Failed to find node\n");
    }

    res = { new Double_linked_node<val_type>(val) };

    if (nullptr == res)
    {
      cerr << "Emplace_element: Failed to create result node (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Double_linked_list_ex("Failed to create result node\n");
    }

    Double_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { res };

    res->next_ = { tmp };
    res->prev_ = { after };

    if (nullptr != tmp)
    {
      tmp->prev_ = { res };
    }
    size_++;

    return res;
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
      cerr << "Add_element: Failed to find node " << after << " in double linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
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

  Double_linked_node<val_type>* Add_element(Double_linked_node<val_type> *el, Double_linked_node<val_type> *after) noexcept(false)
  {
    if (nullptr == el)
    {
      cerr << "Add_element: el must not be nullptr in double linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("el must not be nullptr");
    }

    if (nullptr != after && false == Find_node(after))
    {
      cerr << "Add_element: Failed to find node " << after << " in double linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Failed to find node");
    }

    // There were no nodes before
    if (nullptr == first_)
    {
      first_ = { el };
      size_++;

      return first_;
    }

    // Add as a first node
    if (nullptr == after)
    {
      Double_linked_node<val_type> *tmp { first_ };

      first_ = { el };

      el->next_  = { tmp };
      tmp->prev_ = { el };

      size_++;

      return first_;
    }

    Double_linked_node<val_type> *tmp { after->next_ };

    after->next_ = { el };

    el->next_ = { tmp };
    el->prev_ = { after };

    if (nullptr != tmp)
    {
      tmp->prev_ = { el };
    }
    size_++;

    return after->next_;
  }

  bool Delete_element_cs(Double_linked_node<val_type> *el)
  {
    if (nullptr == el)
    {
      cerr << "Delete_element_cs: Node to delete wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      return false;
    }

    if (false == Find_node(el))
    {
      cerr << "Delete_element_cs: Failed to find node " << el << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

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
      cerr << "Delete_element_cs: node " << el << " isn't the first node, but doesn't have parents (" << __FILE__ << ' ' << __LINE__ << ")\n";
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

  void Delete_element_ex(Double_linked_node<val_type> *el) noexcept(false)
  {
    if (nullptr == el)
    {
      cerr << "Delete_element_ex: Node to delete wasn't provided (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Double_linked_list_ex("Node to delete wasn't provided");
    }

    if (false == Find_node(el))
    {
      cerr << "Delete_element_ex: Failed to find node " << el << " in single linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Double_linked_list_ex("Failed to find node in single linked list");
    }

    if (el == first_)
    {
      first_ = { el->next_ };
      el->~Double_linked_node();
      size_--;

      return;
    }

    Double_linked_node<val_type> *parent { el->prev_ };

    if (nullptr == parent)
    {
      cerr << "Delete_element_ex: node " << el << " isn't the first node, but doesn't have parents (" << __FILE__ << ' ' << __LINE__ << ")\n";
      
      throw Single_linked_list_ex("Parent is nullptr");
    }

    if (nullptr == el->next_)
    {
      // No need to connect the el's child with a new parent

      parent->next_ = { nullptr };
      el->~Double_linked_node();
      size_--;

      return;
    }
    else
    {
      // el's child will need a new parent -- el's parent

      parent->next_ = { el->next_ };
      el->~Double_linked_node();
      size_--;

      return;
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
      cerr << "Find_value_first_occur: Double linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

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
    cout << "Find_value_first_occur: Failed to find node vith val " << val << " in double linked list " << this << " (" << __FILE__ << ' ' << __LINE__ << ")\n";
    return false;
  }

  Double_linked_node<val_type>* Find_value_first_occur(val_type val) noexcept(false)
  {
    if (nullptr == first_)
    {
      cerr << "Find_value_first_occur: Double linked list " << this << "doesn't have any nodes (" << __FILE__ << ' ' << __LINE__ << ")\n";

      throw Double_linked_list_ex("Double linked list doesn't have any nodes");
    }

    Double_linked_node<val_type> *curr { first_ };

    while (nullptr != curr)
    {
      if (curr->val_ == val)
      {
        return curr;
      }

      curr = curr->next_;
    }

    throw Double_linked_list_ex("Failed to find node in double linked list");
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

      throw Double_linked_list_ex("Double linked list has no elements");
    }

    if (index >= size_)
    {
      cerr << "Double_linked_node operator[]: index " << index << " is out of bounds (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Double linked list: index is out of bounds");
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
        
        throw Double_linked_list_ex("Can't access element with this index");
      }
    }

    return curr;
  }

  size_t Get_size() const noexcept
  {
    return size_;
  }

  bool Move_after_cs(size_t what, size_t after)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_after_cs: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (after >= size_)
    {
      cerr << "Double_linked_node Move_after_cs: after (" << after << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
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

    return Move_cs(what_ptr, after_ptr);
  }

  void Move_after_ex(size_t what, size_t after) noexcept(false)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_after_ex: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_after_ex: 'what' must be < size");
    }

    if (after >= size_)
    {
      cerr << "Double_linked_node Move_after_ex: after (" << after << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_after_ex: 'after' must be < size");
    }

    if (what == after)
    {
      return;
    }

    if (abs(what - after) == 1 && what > after)
    {
      return;
    }

    Double_linked_node<val_type> *what_ptr  { operator[](what) };
    Double_linked_node<val_type> *after_ptr { operator[](after) };

    return Move_ex(what_ptr, after_ptr);
  }

  bool Move_before_cs(size_t what, size_t before)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_before_cs: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (before >= size_)
    {
      cerr << "Double_linked_node Move_before_cs: before (" << before << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
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

      return Move_cs(what_ptr, after_ptr);
    }
    else
    {
      return Move_cs(what_ptr, nullptr);
    }
  }

  void Move_before_ex(size_t what, size_t before) noexcept(false)
  {
    if (what >= size_)
    {
      cerr << "Double_linked_node Move_before_ex: what (" << what << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_before_ex: 'what' must be < size");
    }

    if (before >= size_)
    {
      cerr << "Double_linked_node Move_before_ex: before (" << before << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_before_ex: 'before' must be < size");
    }

    if (what == before)
    {
      return;
    }

    if (abs(what - before) == 1 && what < before)
    {
      return;
    }

    Double_linked_node<val_type> *what_ptr { operator[](what) };

    if (before > 0)
    {
      Double_linked_node<val_type> *after_ptr { operator[](before - 1) };

      return Move_cs(what_ptr, after_ptr);
    }
    else
    {
      return Move_cs(what_ptr, nullptr);
    }
  }

  bool Swap_vals_cs(size_t a, size_t b)
  {
    if (a >= size_)
    {
      cerr << "Double_linked_node Swap_vals_cs: a (" << a << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (b >= size_)
    {
      cerr << "Double_linked_node Swap_vals_cs: b (" << b << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    Double_linked_node<val_type> *a_ptr { operator[](a) };
    Double_linked_node<val_type> *b_ptr { operator[](b) };

    return Swap_vals_cs(a_ptr, b_ptr);
  }

  void Swap_vals_ex(size_t a, size_t b)
  {
    if (a >= size_)
    {
      cerr << "Double_linked_node Swap_vals_ex: a (" << a << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'a' must be < size");
    }

    if (b >= size_)
    {
      cerr << "Double_linked_node Swap_vals_ex: b (" << b << ") must be < size (" << size_ << ") (" << " (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'b' must be < size");
    }

    if (a == b)
    {
      return;
    }

    Double_linked_node<val_type> *a_ptr{ operator[](a) };
    Double_linked_node<val_type> *b_ptr{ operator[](b) };

    return Swap_vals_ex(a_ptr, b_ptr);
  }

  bool Swap_vals_cs(Double_linked_node<val_type>* a, Double_linked_node<val_type>* b)
  {
    if (nullptr == a)
    {
      cerr << "Double_linked_node Swap_vals_cs: a (" << a << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (nullptr == b)
    {
      cerr << "Double_linked_node Swap_vals_cs: b (" << b << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (a == b)
    {
      return true;
    }

    if (false == Find_node(a))
    {
      cerr << "Double_linked_node Swap_vals_cs: node a (" << a << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(b))
    {
      cerr << "Double_linked_node Swap_vals_cs: node b (" << b << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    val_type tmp{ a->val_ };
    a->val_ = { b->val_ };
    b->val_ = { tmp };

    return true;
  }

  void Swap_vals_ex(Double_linked_node<val_type>* a, Double_linked_node<val_type>* b)
  {
    if (nullptr == a)
    {
      cerr << "Double_linked_node Swap_vals_ex: a (" << a << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'a' must not be nullptr");
    }

    if (nullptr == b)
    {
      cerr << "Double_linked_node Swap_vals_ex: b (" << b << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'b' must not be nullptr");
    }

    if (a == b)
    {
      return;
    }

    if (false == Find_node(a))
    {
      cerr << "Double_linked_node Swap_vals_ex: node a (" << a << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'a' must be a part of linked list");
    }

    if (false == Find_node(b))
    {
      cerr << "Double_linked_node Swap_vals_ex: node b (" << b << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Swap_vals_ex: 'b' must be a part of linked list");
    }

    val_type tmp { a->val_ };
    a->val_ = { b->val_ };
    b->val_ = { tmp };

    return;
  }

  private:

  bool Move_cs(Double_linked_node<val_type>* what, Double_linked_node<val_type>* after)
  {
    if (nullptr == what)
    {
      cerr << "Double_linked_node Move_cs: what (" << what << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      return false;
    }

    if (false == Find_node(what))
    {
      cerr << "Double_linked_node Move_cs: node what (" << what << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
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

  void Move_ex(Double_linked_node<val_type>* what, Double_linked_node<val_type>* after)
  {
    if (nullptr == what)
    {
      cerr << "Double_linked_node Move: what (" << what << ") must not be nullptr (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_ex: 'what' must not be nullptr");
    }

    if (false == Find_node(what))
    {
      cerr << "Double_linked_node Move: node what (" << what << ") must be a part of linked list (" << this << ") (" << __FILE__ << ", " << __LINE__ << ")\n";
      
      throw Double_linked_list_ex("Move_ex: 'what' must be a part of linked list");
    }

    if (what == after)
    {
      return;
    }

    Double_linked_node<val_type> *what_prev_old  { what->prev_ };
    Double_linked_node<val_type> *what_next_old  { what->next_ };
    Double_linked_node<val_type> *after_next_old {};

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

    return;
  }

  size_t size_;
  Double_linked_node<val_type> *first_;
};

int main()
{
  do
  {
    cout << "Single:\nError codes:\n";

    Single_linked_list<double> sl;

    double _one  { 3.14 }, _two { 2.0 }, _three { 3.0 }, _four{ 4.0 };
    double _five { 5.0 },  _six { 4.5 }, _seven { 1.77 };

    Single_linked_node<double> *node_1   { new Single_linked_node<double>(_seven) };

    Single_linked_node<double> *emplaced { new Single_linked_node<double> };

    cout << "Add " << _one << ", ";

    if (false == sl.Emplace_element(_one, nullptr, &emplaced))
    {
      return -1;
    }

    cout << "Add " << _two << ", ";

    if (false == sl.Emplace_element(_two, emplaced, &emplaced))
    {
      return -1;
    }

    cout << "Add " << _three << ", ";

    if (false == sl.Emplace_element(_three, emplaced, &emplaced))
    {
      return -1;
    }

    Single_linked_node<double> *four { new Single_linked_node<double> };

    cout << "Add " << _four << ", ";

    if (false == sl.Emplace_element(_four, emplaced, &four))
    {
      return -1;
    }

    cout << "Add " << _five << ", ";
  
    if (false == sl.Emplace_element(_five, four, &emplaced))
    {
      return -1;
    }

    cout << "Add " << _six << ", ";

    if (false == sl.Emplace_element(_six, four, &emplaced))
    {
      return -1;
    }

    cout << "Add " << _seven << endl;

    if (false == sl.Add_element(&node_1, emplaced))
    {
      return -1;
    }

    Single_linked_node<double> *curr { sl.Get_first() };

    cout << "Orig: ";

    while (nullptr != curr)
    {
      cout << curr->Get_val() << ' ';
      curr = curr->Get_next();
    }

    cout << ", size " << sl.Get_size() << endl;

    Single_linked_node<double> *parent { new Single_linked_node<double> };

    if (sl.Find_parent(emplaced, &parent) && nullptr != parent)
    {
      cout << "Parent of " << emplaced->Get_val() << ": " << parent->Get_val();
    }

    cout << endl;

    cout << "Move 5 after 6: " << sl.Move_after_cs(5, 6) << endl;

    for (size_t i{}; i < sl.Get_size(); ++i)
    {
      cout << sl[i]->Get_val() << ' ';
    }

    cout << endl;

    Single_linked_node<double> *found { new Single_linked_node<double> };

    if (sl.Find_value_first_occur(5.0, &found) && nullptr != found)
    {
      sl.Delete_element_cs(found);
    }

    cout << "size " << sl.Get_size() << endl;

    curr = sl.Get_first();

    cout << "Print_1: ";

    while (nullptr != curr)
    {
      cout << curr->Get_val() << ' ';
      curr = curr->Get_next();
    }

    cout << endl;

    cout << "Print_2: ";

    for (size_t i{}; i < sl.Get_size(); ++i)
    {
      cout << sl[i]->Get_val() << ' ';
    }

    cout << endl;
  }
  while (false);

  do
  {
    cout << "\nSingle:\nExceptions:\n";

    Single_linked_list<double> sl;

    double _one  { 3.14 }, _two { 2.0 }, _three { 3.0 }, _four { 4.0 };
    double _five { 5.0 },  _six { 4.5 }, _seven { 1.77 };

    Single_linked_node<double> *node_1 { new Single_linked_node<double>(_seven) };

    Single_linked_node<double> *emplaced { nullptr };

    cout << "Add " << _one << ", ";

    try
    {
      emplaced = { sl.Emplace_element(_one, nullptr) };

      cout << "Add " << _two << ", ";

      emplaced = { sl.Emplace_element(_two, emplaced) };

      cout << "Add " << _three << ", ";

      emplaced = { sl.Emplace_element(_three, emplaced) };

      Single_linked_node<double> *four{ nullptr };

      cout << "Add " << _four << ", ";

      four = { sl.Emplace_element(_four, emplaced) };

      cout << "Add " << _five << ", ";

      emplaced = { sl.Emplace_element(_five, four) };

      cout << "Add " << _six << ", ";

      emplaced = { sl.Emplace_element(_six, four) };

      cout << "Add " << _seven << endl;

      node_1 = { sl.Add_element(node_1, emplaced) };

      Single_linked_node<double> *curr{ sl.Get_first() };

      cout << "Orig: ";

      while (nullptr != curr)
      {
        cout << curr->Get_val() << ' ';
        curr = { curr->Get_next() };
      }

      cout << ", size " << sl.Get_size() << endl;

      Single_linked_node<double> *parent{ nullptr };

      parent = { sl.Find_parent(emplaced) };

      cout << "Parent of " << emplaced->Get_val() << ": " << parent->Get_val();

      cout << endl;

      cout << "Move 1 after 3: ";
      sl.Move_after_ex(1, 3);
      cout << "success";

      cout << endl;

      for (size_t i{}; i < sl.Get_size(); ++i)
      {
        cout << sl[i]->Get_val() << ' ';
      }

      cout << endl;

      Single_linked_node<double> *found{ new Single_linked_node<double> };

      found = { sl.Find_value_first_occur(5.0) };

      if (nullptr != found)
      {
        sl.Delete_element_ex(found);
      }

      cout << "size " << sl.Get_size() << endl;

      curr = sl.Get_first();

      cout << "Print_1: ";

      while (nullptr != curr)
      {
        cout << curr->Get_val() << ' ';
        curr = curr->Get_next();
      }

      cout << endl;

      cout << "Print_2: ";

      for (size_t i{}; i < sl.Get_size(); ++i)
      {
        cout << sl[i]->Get_val() << ' ';
      }

      cout << endl;
    }
    catch (Single_linked_list_ex& ex)
    {
      cerr << "Exception: " << ex.what() << endl;

      cin.get();

      return -1;
    }
  }
  while (false);

  do
  {
    cout << "\nDouble:\nError codes:\n";

    Double_linked_list<double> dl;

    double _one  { 3.14 }, _two { 2.0 },  _three { 3.0 }, _four { 4.0 };
    double _five { 5.0 },  _six  { 4.5 }, _seven { 1.77 };

    Double_linked_node<double> *node_2     { new Double_linked_node<double>(_seven) };

    Double_linked_node<double> *emplaced_2 { new Double_linked_node<double> };

    cout << "Add " << _one << ", ";

    if (false == dl.Emplace_element(_one, nullptr, &emplaced_2))
    {
      return -1;
    }

    cout << "Add " << _two << ", ";

    if (false == dl.Emplace_element(_two, emplaced_2, &emplaced_2))
    {
      return -1;
    }

    cout << "Add " << _three << ", ";

    if (false == dl.Emplace_element(_three, emplaced_2, &emplaced_2))
    {
      return -1;
    }

    Double_linked_node<double> *four_2 { new Double_linked_node<double> };

    cout << "Add " << _four << ", ";

    if (false == dl.Emplace_element(_four, emplaced_2, &four_2))
    {
      return -1;
    }

    cout << "Add " << _five << ", ";

    if (false == dl.Emplace_element(_five, four_2, &emplaced_2))
    {
      return -1;
    }

    cout << "Add " << _six << ", ";

    if (false == dl.Emplace_element(_six, four_2, &emplaced_2))
    {
      return -1;
    }

    cout << "Add " << _seven << endl;

    if (false == dl.Add_element(&node_2, emplaced_2))
    {
      return -1;
    }

    Double_linked_node<double> *curr_2 { dl.Get_first() };

    cout << "Orig: ";

    while (nullptr != curr_2)
    {
      cout << curr_2->Get_val() << ' ';
      curr_2 = { curr_2->Get_next() };
    }

    cout << ", size " << dl.Get_size() << endl;

    Double_linked_node<double> *parent_2 { nullptr };

    if (nullptr != emplaced_2)
    {
      parent_2 = { emplaced_2->Get_prev() };
    }

    if (nullptr != parent_2)
    {
      cout << "Parent of " << emplaced_2->Get_val() << ": " << emplaced_2->Get_prev()->Get_val();
    }

    cout << endl;

    cout << "Move 6 after 0: " << boolalpha << dl.Move_after_cs(6, 0) << endl;

    for (size_t i{}; i < dl.Get_size(); ++i)
    {
      cout << dl[i]->Get_val() << ' ';
    }

    cout << endl;

    Double_linked_node<double> *found_2 { new Double_linked_node<double> };

    if (dl.Find_value_first_occur(4.5, &found_2) && nullptr != found_2)
    {
      dl.Delete_element_cs(found_2);
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

  do
  {
    cout << "\nDouble:\nExceptions:\n";

    Double_linked_list<double> dl;

    double _one{ 3.14 }, _two{ 2.0 }, _three{ 3.0 }, _four{ 4.0 };
    double _five{ 5.0 }, _six{ 4.5 }, _seven{ 1.77 };

    Double_linked_node<double> *node_2 { new Double_linked_node<double>(_seven) };

    Double_linked_node<double> *emplaced_2 { nullptr };

    cout << "Add " << _one << ", ";

    try
    {
      emplaced_2 = { dl.Emplace_element(_one, nullptr) };

      cout << "Add " << _two << ", ";

      emplaced_2 = { dl.Emplace_element(_two, emplaced_2) };

      cout << "Add " << _three << ", ";

      emplaced_2 = { dl.Emplace_element(_three, emplaced_2) };

      Double_linked_node<double> *four_2{ new Double_linked_node<double> };

      cout << "Add " << _four << ", ";

      four_2 = { dl.Emplace_element(_four, emplaced_2) };

      cout << "Add " << _five << ", ";

      emplaced_2 = { dl.Emplace_element(_five, four_2) };

      cout << "Add " << _six << ", ";

      emplaced_2 = { dl.Emplace_element(_six, four_2) };

      cout << "Add " << _seven << endl;

      node_2 = { dl.Add_element(node_2, emplaced_2) };

      Double_linked_node<double> *curr_2 { dl.Get_first() };

      cout << "Orig: ";

      while (nullptr != curr_2)
      {
        cout << curr_2->Get_val() << ' ';
        curr_2 = { curr_2->Get_next() };
      }

      cout << ", size " << dl.Get_size() << endl;

      Double_linked_node<double> *parent_2{ nullptr };

      if (nullptr != emplaced_2)
      {
        parent_2 = { emplaced_2->Get_prev() };
      }

      if (nullptr != parent_2)
      {
        cout << "Parent of " << emplaced_2->Get_val() << ": " << emplaced_2->Get_prev()->Get_val();
      }

      cout << endl;

      cout << "Move 1 after 4: ";
      dl.Move_after_ex(1, 4);
      cout << "success";

      cout << endl;

      for (size_t i{}; i < dl.Get_size(); ++i)
      {
        cout << dl[i]->Get_val() << ' ';
      }

      cout << endl;

      Double_linked_node<double> *found_2 { nullptr };

      found_2 = { dl.Find_value_first_occur(4.5) };

      if (nullptr != found_2)
      {
        dl.Delete_element_ex(found_2);
      }

      cout << "size " << dl.Get_size() << endl;

      curr_2 = dl.Get_first();

      while (nullptr != curr_2)
      {
        cout << curr_2->Get_val() << ' ';
        curr_2 = { curr_2->Get_next() };
      }

      cout << endl;

      for (size_t i{}; i < dl.Get_size(); ++i)
      {
        cout << dl[i]->Get_val() << ' ';
      }

      cout << endl;
    }
    catch (Double_linked_list_ex& ex)
    {
      cerr << "Exception: " << ex.what() << endl;

      cin.get();

      return -1;
    }
  }
  while (false);

  cin.get();

  return 0;
}
