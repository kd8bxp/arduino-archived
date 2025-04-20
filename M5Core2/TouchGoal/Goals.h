#pragma once

#define TEXT_TOP        25
#define TEXT_CENTER     160
#define TEXT_HEIGHT     32
#define TEXT_FONT       4

#define TEST_DURRATION  8000

class Goal {
  public:
    Goal();
    bool          test();
    bool          passed();
    const char*   getName();
    virtual void  event_handler(Event& evt) = 0;
  protected:
    String        name;
    uint32_t      start_time;
    bool          success;
};

class TapAGoal : public Goal {
  public:
    TapAGoal();
    void  event_handler(Event& evt);
};

class TapBGoal : public Goal {
  public:
    TapBGoal();
    void  event_handler(Event& evt);
};

class DoubleTapAGoal : public Goal {
  public:
    DoubleTapAGoal();
    void  event_handler(Event& evt);
};

class DoubleTapBGoal : public Goal {
  public:
    DoubleTapBGoal();
    void  event_handler(Event& evt);
};

class TapBackgroundGoal : public Goal {
  public:
    TapBackgroundGoal();
    void  event_handler(Event& evt);
};

class DoubleTapBackgroundGoal : public Goal {
  public:
    DoubleTapBackgroundGoal();
    void  event_handler(Event& evt);
};

class DragFromAtoBGoal : public Goal {
  public:
    DragFromAtoBGoal();
    void  event_handler(Event& evt);
};

class DragFromBtoAGoal : public Goal {
  public:
    DragFromBtoAGoal();
    void  event_handler(Event& evt);
};

class DragFromAtoBackgroundGoal : public Goal {
  public:
    DragFromAtoBackgroundGoal();
    void  event_handler(Event& evt);
};

class DragFromBtoBackgroundGoal : public Goal {
  public:
    DragFromBtoBackgroundGoal();
    void  event_handler(Event& evt);
};

class DragFromBackgroundtoAGoal : public Goal {
  public:
    DragFromBackgroundtoAGoal();
    void  event_handler(Event& evt);
  private:
    bool can_succeed;
};

class DragFromBackgroundtoBGoal : public Goal {
  public:
    DragFromBackgroundtoBGoal();
    void  event_handler(Event& evt);
  private:
    bool can_succeed;
};

class SwipeUpGoal : public Goal {
  public:
    SwipeUpGoal();
    void  event_handler(Event& evt);
};

class SwipeDownGoal : public Goal {
  public:
    SwipeDownGoal();
    void  event_handler(Event& evt);
};

class SwipeLeftGoal : public Goal {
  public:
    SwipeLeftGoal();
    void  event_handler(Event& evt);
};

class SwipeRightGoal : public Goal {
  public:
    SwipeRightGoal();
    void  event_handler(Event& evt);
};
