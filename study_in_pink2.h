/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//Forward declaration
 class MovingObject;
 class Position;
 class Configuration;
 class Map;
 class Node;
 class BaseBag;
 class BaseItem;
 class BaseBag;
 class SherlockBag;
 class WatsonBag;
 class Character;
 class Robot;
 class Criminal;
 class RobotS;
 class RobotW;
 class RobotSW;
 class RobotC;

 class ArrayMovingObject;
 class StudyPinkProgram;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
	friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const = 0;
    virtual int getReqExp() const;
};

class Path : public MapElement {
	friend class TestStudyInPink;
public:
	Path();
	ElementType getType() const;
};

class Wall : public MapElement {
	friend class TestStudyInPink;
public:
	Wall();
	ElementType getType() const;
};

class FakeWall : public MapElement {
	friend class TestStudyInPink;
private:
	int req_exp;
public:
    FakeWall(int in_req_exp);
	int getReqExp() const;
	ElementType getType() const;
};

class Position {
	friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

    Position(int r=0, int c=0);
    Position(const string & str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    Position up();
    Position left();
    Position down();
    Position right();
//    Position up2();
//    Position left2();
//    Position down2();
//    Position right2();
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;
};

class Map {
	friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    MapElement*** map;
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls,
    	int num_fake_walls, Position * array_fake_walls);
    ~Map();
    bool isValid(const Position & pos, MovingObject * mv_obj) const;
    int getRows() const;
    int getCols() const;
};


class MovingObject {
	friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual int getExp() const = 0;
    virtual int getHp() const = 0;
    virtual void setExp(int e) = 0;
    virtual void setHp(int h) = 0;
    virtual string getName();
    virtual void setItem(BaseItem* item);
    virtual BaseItem* getItem() const;
    virtual int getSizeMovingRule() const;
};

class Robot : public MovingObject {
	friend class TestStudyInPink;
protected:
	RobotType robot_type;
	BaseItem* item;
public:
	Robot(int index , const Position & init_pos , Map * map , const string & name,
			RobotType robot_type);
	virtual ~Robot();

	virtual int getDistance() const = 0;
	virtual RobotType getRobotType() const = 0;
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual void setItem(BaseItem* item) = 0;
    virtual BaseItem* getItem() const = 0;

	int getExp() const;
	int getHp() const;
	void setExp(int e);
	void setHp(int h);

};

class Node {
	friend class TestStudyInPink;
public:
    BaseItem* item;
    Node* next;
    ~Node();
};

class BaseBag {
	friend class TestStudyInPink;
protected:
    Node* head;
    Character* obj;
    int maxItems;
public:
    BaseBag(Character* obj, int maxItems);
    virtual ~BaseBag();

    virtual bool insert(BaseItem* item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    virtual string str() const;
    int getCount() const;
    bool isAvailable(ItemType itemType);
    bool isFull() const;
};

class Character : public MovingObject {
	friend class TestStudyInPink;

public:
    Character(int index, const Position pos, Map* map, const string& name);
    virtual ~Character();

    virtual bool addItem(BaseItem* item);
    virtual BaseItem* useItem();
    virtual BaseItem* useItem(ItemType itemType);

    virtual int getExp() const = 0;
    virtual int getHp() const = 0;
    virtual void setExp(int e) = 0;
    virtual void setHp(int h) = 0;

    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual BaseBag * getBag() const;
    virtual int getSizeMovingRule() const;
};

class Sherlock : public Character {
	friend class TestStudyInPink;
private:
    int hp;
    int exp;
    string moving_rule;

    BaseBag * bag;

public:
    static int ruleIndex;

    Sherlock(int index, const string & moving_rule, const Position & init_pos,
    		Map * map, int init_hp, int init_exp);
    ~Sherlock();
    Position getNextPosition();
    void move();
    Position getCurrentPosition() const;
    int getExp() const;
    int getHp() const;
    void setExp(int e);
    void setHp(int h);
    bool addItem(BaseItem* item);
    BaseItem* useItem();
    BaseItem* useItem(ItemType itemType);
    string str() const;
    BaseBag * getBag() const;
    int getSizeMovingRule() const;
};

class Watson : public Character {
	friend class TestStudyInPink;
private:
	int hp;
	int exp;
	string moving_rule;

	BaseBag * bag;

public:
	static int ruleIndex;

    Watson(int index, const string & moving_rule, const Position & init_pos,
    		Map * map, int init_hp, int init_exp);
    ~Watson();
    Position getNextPosition();
    void move();
    Position getCurrentPosition() const;
    int getExp() const;
    int getHp() const;
    void setExp(int e);
    void setHp(int h);
    bool addItem(BaseItem* item);
    BaseItem* useItem();
    BaseItem* useItem(ItemType itemType);
    string str() const;
    BaseBag * getBag() const;
    int getSizeMovingRule() const;

};

class SherlockBag : public BaseBag {
	friend class TestStudyInPink;
public:
    SherlockBag(Sherlock* sherlock);
};

class WatsonBag : public BaseBag {
	friend class TestStudyInPink;
public:
    WatsonBag(Watson* watson);
};

class Criminal : public Character {
	friend class TestStudyInPink;
private:
    Sherlock *sherlock;
    Watson *watson;

public:
    Criminal(int index, const Position & init_pos, Map * map,
    		Sherlock * sherlock, Watson * watson);
    Position getNextPosition();
    void move();
    Position getCurrentPosition() const;
    Position getPreviousPosition();
    string str() const;
    int getExp() const;
    int getHp() const;
    void setExp(int e);
    void setHp(int h);
};

class RobotC: public Robot {
	friend class TestStudyInPink;
private:
	Criminal* criminal;

public:
	RobotC(int index , const Position & init_pos , Map * map , Criminal * criminal);


	~RobotC();

	Position getNextPosition();
    Position getCurrentPosition() const;
    string str() const;
    int getDistance() const;
    int getDistance(Sherlock* sherlock) const;
    int getDistance(Watson* watson) const;
	void move();
	RobotType getRobotType() const;
	void setItem(BaseItem* item);
	BaseItem* getItem() const;
};

class RobotS: public Robot {
	friend class TestStudyInPink;
private:
	Criminal* criminal;
	Sherlock* sherlock;
public:
	RobotS(int index , const Position & init_pos , Map * map , Criminal * criminal, Sherlock * sherlock);
	~RobotS();

	Position getNextPosition();
    Position getCurrentPosition() const;
    string str() const;
    int getDistance() const;
	void move();
	RobotType getRobotType() const;
	void setItem(BaseItem* item);
	BaseItem* getItem() const;
};

class RobotW: public Robot {
	friend class TestStudyInPink;
private:
	Criminal* criminal;
	Watson* watson;

public:
	RobotW(int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson);
	~RobotW();

	Position getNextPosition();
    Position getCurrentPosition() const;
    string str() const;
    int getDistance() const;
	void move();
	RobotType getRobotType() const;
	void setItem(BaseItem* item);
	BaseItem* getItem() const;
};

class RobotSW: public Robot {
	friend class TestStudyInPink;
private:
	Criminal* criminal;
	Sherlock* sherlock;
	Watson* watson;

public:
	RobotSW(int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson);
	~RobotSW();

	Position getNextPosition();
    Position getCurrentPosition() const;
    string str() const;
    int getDistance() const;
	void move();
	RobotType getRobotType() const;
	void setItem(BaseItem* item);
	BaseItem* getItem() const;
};

class BaseItem {
	friend class TestStudyInPink;
protected:
	ItemType type;

public:
	BaseItem(ItemType type);
    virtual ~BaseItem();

    virtual bool canUse(Character* obj, Robot* robot) = 0;
    virtual void use(Character* obj, Robot* robot) = 0;
    virtual ItemType getType() = 0;
};

class MagicBook: public BaseItem {
	friend class TestStudyInPink;
public:
	MagicBook();
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot) ;
    ItemType getType() ;
};

class EnergyDrink: public BaseItem {
	friend class TestStudyInPink;
public:
	EnergyDrink();
    bool canUse(Character* obj, Robot* robot) ;
    void use(Character* obj, Robot* robot) ;
    ItemType getType() ;
};

class FirstAid: public BaseItem {
	friend class TestStudyInPink;
public:
	FirstAid();
    bool canUse(Character* obj, Robot* robot) ;
    void use(Character* obj, Robot* robot) ;
    ItemType getType() ;
};

class ExcemptionCard: public BaseItem {
	friend class TestStudyInPink;
public:
	ExcemptionCard();
    bool canUse(Character* obj, Robot* robot) ;
    void use(Character* obj, Robot* robot) ;
    ItemType getType() ;
};

class PassingCard: public BaseItem {
	friend class TestStudyInPink;
private:
    string challenge;
public:
    PassingCard(string challenge);
    bool canUse(Character* obj, Robot* robot) ;
    void use(Character* obj, Robot* robot) ;
    ItemType getType() ;
};



class Configuration {
	friend class TestStudyInPink;
    friend class StudyPinkProgram;
private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls;
    int num_fake_walls;
    Position* arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string& filepath);
    ~Configuration();

    string str() const;
    string extractSubstring(const string& value, int index, int num);
};

class ArrayMovingObject {
	friend class TestStudyInPink;
private:
	MovingObject** arr_mv_objs;
	static int count;
	int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();

    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    string str() const;
    static int size();

};

class StudyPinkProgram {
	friend class TestStudyInPink;
private:
    Configuration * config;
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    Map * map;
    ArrayMovingObject * arr_mv_objs;
    int arrest;
    Criminal * prev_criminal;

public:
    StudyPinkProgram(const string & config_file_path);
    ~StudyPinkProgram();

    bool isStop() const;
    void printResult() const;
    void printStep(int si) const;
    int cardinal(int n);
    void run(bool verbose);
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
