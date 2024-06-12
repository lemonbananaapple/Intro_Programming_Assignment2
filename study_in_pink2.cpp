#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const Position Position::npos(-1, -1);

int ArrayMovingObject::count = 0;

int Sherlock::ruleIndex = 0;
int Watson::ruleIndex = 0;

MapElement::MapElement(ElementType in_type) : type(in_type) {}

MapElement::~MapElement() {}

int MapElement::getReqExp() const{
	return 0;
}

Path::Path() : MapElement(PATH) {}

ElementType Path::getType() const { return type; }

Wall::Wall() : MapElement(WALL) {}

ElementType Wall::getType() const { return type; }

FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(max(min(in_req_exp, 900), 0)) {}

int FakeWall::getReqExp() const { return req_exp; }

ElementType FakeWall::getType() const { return type; }

Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls,
    	int num_fake_walls, Position * array_fake_walls)
    	: num_rows(num_rows), num_cols(num_cols){
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j) {
            map[i][j] = new Path();
        }
    }
    for (int i = 0; i < num_walls; ++i) {
        int row = array_walls[i].getRow();
        int col = array_walls[i].getCol();
        map[row][col] = new Wall();
    }
    for (int i = 0; i < num_fake_walls; ++i) {
        int row = array_fake_walls[i].getRow();
        int col = array_fake_walls[i].getCol();
        map[row][col] = new FakeWall((row * 257 + col *139 + 89)%900 + 1 );
    }
}

Map::~Map() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
    map = nullptr;
}

int Map::getRows() const{
	return num_rows;
}

int Map::getCols() const{
	return num_cols;
}

bool Map::isValid(const Position & pos, MovingObject * mv_obj) const {
	if(!((pos.getRow() >= 0 && pos.getRow() < this->num_rows )&& (pos.getCol() >= 0 && pos.getCol() < this->num_cols))){
		return false;
	}

	if(pos.getRow() == Position::npos.getRow() || pos.getCol() == Position::npos.getCol()){
		return false;
	}
    if (map[pos.getRow()][pos.getCol()]->getType() == PATH){
        return true;
    }
    if(map[pos.getRow()][pos.getCol()]->getType() == WALL){
        return false;
    }
    if(map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL){
        if (dynamic_cast<Watson*>(mv_obj)){
            if(map[pos.getRow()][pos.getCol()]->getReqExp() < mv_obj->getExp()){
                return true;
            } else {
                return false;
            }
        }
        else{
            return true;
        }
    }
    return false;
}

Position::Position(int r, int c): r(r), c(c){}

Position::Position(const string & str_pos){
    int pos1 = str_pos.find('(');
    int pos2 = str_pos.find(',');
    int pos3 = str_pos.find(')');
    if (pos1 != static_cast<int>(string::npos) && pos2 != static_cast<int>(string::npos) && pos3 != static_cast<int>(string::npos)) {
        r = stoi(str_pos.substr(pos1 + 1, pos2 - pos1 - 1));
        c = stoi(str_pos.substr(pos2 + 1, pos3 - pos2 - 1));
    } else {
        r = -1;
        c = -1;
    }
}

int Position::getRow() const{
    return r;
}

int Position::getCol() const{
    return c;
}

void Position::setRow(int r){
    this->r = r;
}

void Position::setCol(int c){
    this->c = c;
}

Position Position::up(){
    Position newPosition = *this;
    newPosition.setRow(newPosition.getRow() - 1);
    return newPosition;
}

Position Position::left(){
    Position newPosition = *this;
    newPosition.setCol(newPosition.getCol() - 1);
    return newPosition;
}

Position Position::down(){
    Position newPosition = *this;
    newPosition.setRow(newPosition.getRow() + 1);
    return newPosition;
}

Position Position::right(){
    Position newPosition = *this;
    newPosition.setCol(newPosition.getCol() + 1);
    return newPosition;
}

string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const{
    return r == in_r && c == in_c;
}

bool Position::isEqual(Position pos) const{
    return r == pos.getRow() && c == pos.getCol();
}


MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name)
    : index(index), pos(pos), map(map) ,name(name){}

MovingObject::~MovingObject(){

}

string MovingObject::getName(){
    return name;
}
void MovingObject::setItem(BaseItem* item){

}

BaseItem* MovingObject::getItem() const{
	return NULL;
}

int MovingObject::getSizeMovingRule() const{
	return 0;
}

Robot::Robot(int index , const Position & init_pos , Map * map , const string & name,
		RobotType robot_type)
	: MovingObject{index, init_pos, map, name}, robot_type(robot_type), item(nullptr){}

Robot::~Robot(){
	delete item;
}

int Robot::getExp() const{
	return 0;
}

int Robot::getHp() const{
	return 0;
}

void Robot::setExp(int e){

}

void Robot::setHp(int h){

}

Character::Character(int index, const Position pos, Map* map, const string& name)
    : MovingObject{index, pos, map, name} {}

Character::~Character() {}

bool Character::addItem(BaseItem* item) {
    return false;
}

BaseItem* Character::useItem() {
    return nullptr;
}

BaseItem* Character::useItem(ItemType itemType){
    return nullptr;
}

BaseBag * Character::getBag() const{
	return nullptr;
}

int Character::getSizeMovingRule() const{
	return 0;
}

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos,
    		Map * map, int init_hp, int init_exp)
    : Character{index, init_pos, map, "Sherlock"}, hp(max(min(init_hp, 500), 0)),
	  exp(max(min(init_exp, 900), 0)), moving_rule(moving_rule){
    	bag = new SherlockBag(this);
    }


Position Sherlock::getNextPosition(){
 	Position next_pos = pos;
	if(exp <= 0 || hp <= 0){
		return pos;
	}
	if (moving_rule.empty()) {
			return Position::npos;
		} else{
			if (!map->isValid(pos, this)) {
				return Position::npos;
			} else{

			char nextMove = moving_rule[ruleIndex];
			switch (nextMove) {
				case 'L':
					next_pos.setCol(pos.getCol() - 1);
					break;
				case 'R':
					next_pos.setCol(pos.getCol() + 1);
					break;
				case 'U':
					next_pos.setRow(pos.getRow() - 1);
					break;
				case 'D':
					next_pos.setRow(pos.getRow() + 1);
					break;
				default:
					return Position::npos;
			}
		}
	}
	if (!map->isValid(next_pos, this)) {
		return Position::npos;
	} else{
		return next_pos;
	}

}

void Sherlock::move(){
    Position nextPos = getNextPosition();
    if (map->isValid(nextPos, this)) {
        pos = nextPos;
    }
    ruleIndex = (ruleIndex + 1) % moving_rule.size();

}

Position Sherlock::getCurrentPosition() const {
	return pos;
}

int Sherlock::getExp() const{
	return exp;
}

int Sherlock::getHp() const{
	return hp;
}

void Sherlock::setExp(int e){
	exp = max(min(e, 900), 0);
}

void Sherlock::setHp(int h){
	hp = max(min(h, 500), 0);
}

bool Sherlock::addItem(BaseItem* item) {
	return bag->insert(item);
}

BaseItem* Sherlock::useItem() {
	return bag->get();
}

BaseItem* Sherlock::useItem(ItemType itemType) {
	return bag->get(itemType);
}

string Sherlock::str() const{
	return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() +
	    			";moving_rule=" + moving_rule + "]";
}

BaseBag * Sherlock::getBag() const{
	return bag;
}

int Sherlock::getSizeMovingRule() const{
	return moving_rule.size();
}

Sherlock::~Sherlock(){
	delete bag;
	ruleIndex = 0;

}

Watson::Watson(int index, const string & moving_rule, const Position & init_pos,
    		Map * map, int init_hp, int init_exp)
    : Character{index, init_pos, map, "Watson"}, hp(max(min(init_hp, 500), 0)),exp(max(min(init_exp, 900), 0)),
	  moving_rule(moving_rule){
    	bag = new WatsonBag(this);
    }

Position Watson::getNextPosition(){
	Position next_pos = pos;
	if(exp <= 0 || hp <= 0){
		return pos;
	}
	if (moving_rule.empty()) {
		return Position::npos;
	} else{
		if (!map->isValid(pos, this)) {
			return Position::npos;
		} else{
		char nextMove = moving_rule[ruleIndex];

		switch (nextMove) {
			case 'L':
				next_pos.setCol(pos.getCol() - 1);
				break;
			case 'R':
				next_pos.setCol(pos.getCol() + 1);
				break;
			case 'U':
				next_pos.setRow(pos.getRow() - 1);
				break;
			case 'D':
				next_pos.setRow(pos.getRow() + 1);
				break;
			default:
				return Position::npos;
			}
		}
	}

	if (!map->isValid(next_pos, this)) {
		return Position::npos;
	}

	return next_pos;
}

void Watson::move(){
	Position nextPos = getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
	ruleIndex = (ruleIndex + 1) % moving_rule.size();
}

Position Watson::getCurrentPosition() const {
	return pos;
}

int Watson::getExp() const{
	return exp;
}

int Watson::getHp() const{
	return hp;
}

void Watson::setExp(int e){
	exp = max(min(e, 900), 0);
}

void Watson::setHp(int h){
	hp = max(min(h, 500), 0);
}

bool Watson::addItem(BaseItem* item) {
	return bag->insert(item);
}

BaseItem* Watson::useItem() {
	return bag->get();
}

BaseItem* Watson::useItem(ItemType itemType){
	return bag->get(itemType);
}

string Watson::str() const{
	return "Watson[index=" + to_string(index) + ";pos=" + pos.str() +
			";moving_rule=" + moving_rule + "]";
}

BaseBag * Watson::getBag() const{
	return bag;
}

int Watson::getSizeMovingRule() const{
	return moving_rule.size();
}

Watson::~Watson(){
	delete bag;
	ruleIndex = 0;
}


Criminal::Criminal(int index, const Position & init_pos, Map * map,
    		Sherlock * sherlock, Watson * watson)
    : Character{index, init_pos, map, "Criminal"}, sherlock(sherlock), watson(watson){}

Position Criminal::getNextPosition(){

	if (!map->isValid(this->getCurrentPosition(), this)) {
		return Position::npos;
	} else {
		Position bestPos = this->getCurrentPosition();
		int max_total_dist = -1;


		Position adjacentPos[] = {
			bestPos.up(),
			bestPos.left(),
			bestPos.down(),
			bestPos.right()
		};

		for (const auto& adjPos : adjacentPos) {
			if (map->isValid(adjPos, this)) {
				int adj_sherlock_dist = abs(adjPos.getRow() - sherlock->getCurrentPosition().getRow()) +
										abs(adjPos.getCol() - sherlock->getCurrentPosition().getCol());
				int adj_watson_dist = abs(adjPos.getRow() - watson->getCurrentPosition().getRow()) +
									  abs(adjPos.getCol() - watson->getCurrentPosition().getCol());

				int total_dist = adj_sherlock_dist + adj_watson_dist;
				if (total_dist > max_total_dist) {
					max_total_dist = total_dist;
					bestPos = adjPos;
				}
			}
		}

		return bestPos;
	}
}

void Criminal::move(){
	Position nextPos = this->getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
}

Position Criminal::getCurrentPosition() const {
	return pos;
}

Position Criminal::getPreviousPosition() {
    Position currentPos = getCurrentPosition();
    Position bestPos = currentPos;
    int minTotalDist = 10000;

    Position adjacentPos[] = {
        currentPos.up(),
        currentPos.left(),
        currentPos.down(),
        currentPos.right()
    };

    for (const auto& adjPos : adjacentPos) {
        if (map->isValid(adjPos, this)) {
            int adjSherlockDist = abs(adjPos.getRow() - sherlock->getCurrentPosition().getRow()) +
                                  abs(adjPos.getCol() - sherlock->getCurrentPosition().getCol());
            int adjWatsonDist = abs(adjPos.getRow() - watson->getCurrentPosition().getRow()) +
                                abs(adjPos.getCol() - watson->getCurrentPosition().getCol());

            int totalDist = adjSherlockDist + adjWatsonDist;
            if (totalDist < minTotalDist) {
                minTotalDist = totalDist;
                bestPos = adjPos;
            }
        }
    }

    return bestPos;
}


string Criminal::str() const{
	return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

int Criminal::getExp() const{
	return 0;
}

int Criminal::getHp() const{
	return 0;
}

void Criminal::setExp(int e){

}

void Criminal::setHp(int h){

}

RobotC::RobotC(int index , const Position & init_pos , Map * map , Criminal * criminal)
	: Robot{index, init_pos, map, "RobotC",C}, criminal(criminal){}


RobotC::~RobotC() {
}

Position RobotC::getNextPosition() {
	if (!map->isValid(pos , this)) {
			return Position::npos;
	} else {
		if( (abs(this->pos.getRow() - criminal->getCurrentPosition().getRow()) > 1) || (abs(this->pos.getCol() - criminal->getCurrentPosition().getCol()) > 1) ){
			return criminal->getPreviousPosition();
		} else{
			if ((this->pos.getRow() != criminal->getCurrentPosition().getRow()) || (this->pos.getCol() != criminal->getCurrentPosition().getCol())){
				return criminal->getCurrentPosition();
			} else {
				return Position::npos;
			}
		}

	}
}

Position RobotC::getCurrentPosition() const {
	return pos;
}

string RobotC::str() const {
	return "Robot[pos=" + pos.str() + ";type=" + "C" + ";dist=" + "" + "]";
}

int RobotC::getDistance() const {
	return 0;
}

int RobotC::getDistance(Sherlock* sherlock) const {
	return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) +
				   abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
}

int RobotC::getDistance(Watson* watson) const {
	return abs(pos.getRow() - watson->getCurrentPosition().getRow()) +
	    		   abs(pos.getCol() - watson->getCurrentPosition().getCol());
}

void RobotC::move() {
	Position nextPos = getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
}

RobotType RobotC::getRobotType() const {
	return robot_type;
}

void RobotC::setItem(BaseItem* item){
	this->item = item;
}

BaseItem* RobotC::getItem() const{
	return item;
}

RobotS::RobotS(int index , const Position & init_pos , Map * map , Criminal * criminal, Sherlock * sherlock)
	: Robot{index, init_pos, map, "RobotS",S}, criminal(criminal), sherlock(sherlock){}

RobotS::~RobotS(){
}

Position RobotS::getNextPosition(){

	if (!map->isValid(pos, this) || !map->isValid(sherlock->getCurrentPosition(), sherlock)) {
				return Position::npos;
			} else {
				Position bestPos = this->getCurrentPosition();
				int min_adj_sherlock_dist = 10000;

				Position adjacentPos[] = {
					bestPos.up(),
					bestPos.right(),
					bestPos.down(),
					bestPos.left()
				};

				for (const auto& adjPos : adjacentPos) {
					if (map->isValid(adjPos, this)) {
						int adj_sherlock_dist = abs(adjPos.getRow() - sherlock->getCurrentPosition().getRow()) +
												abs(adjPos.getCol() - sherlock->getCurrentPosition().getCol());

						if (adj_sherlock_dist < min_adj_sherlock_dist) {
							min_adj_sherlock_dist = adj_sherlock_dist;
							bestPos = adjPos;
						}
					}
				}

				return bestPos;
			}
}

Position RobotS::getCurrentPosition() const {
	return pos;
}

string RobotS::str() const {
	return "Robot[pos=" + pos.str() + ";type=" + "S"
	    			+ ";dist=" + to_string(getDistance()) + "]";
}

int RobotS::getDistance() const {
	return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) +
					abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
}

void RobotS::move() {
	Position nextPos = getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
}

RobotType RobotS::getRobotType() const {
	return robot_type;
}

void RobotS::setItem(BaseItem* item){
	this->item = item;
}

BaseItem* RobotS::getItem() const{
	return item;
}

RobotW::RobotW(int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson)
	: Robot{index, init_pos, map, "RobotW",W}, criminal(criminal), watson(watson){}

RobotW::~RobotW(){
}

Position RobotW::getNextPosition(){
	if (!map->isValid(pos, this) || !map->isValid(watson->getCurrentPosition(), watson)) {
		return Position::npos;
	} else {
		int min_adj_watson_dist = 10000;
		Position bestPos = this->getCurrentPosition();
		Position adjacentPos[] = {
			bestPos.up(),
			bestPos.right(),
			bestPos.down(),
			bestPos.left()
		};

		for (const auto& adjPos : adjacentPos) {
			if (map->isValid(adjPos, this)) {
				int adj_watson_dist = abs(adjPos.getRow() - watson->getCurrentPosition().getRow()) +
									  abs(adjPos.getCol() - watson->getCurrentPosition().getCol());

				if (adj_watson_dist < min_adj_watson_dist) {
					min_adj_watson_dist = adj_watson_dist;
					bestPos = adjPos;
				}
			}
		}

		return bestPos;
	}
}

Position RobotW::getCurrentPosition() const {
	return pos;
}

string RobotW::str() const {
	return "Robot[pos=" + pos.str() + ";type=" + "W"
			+ ";dist=" + to_string(getDistance()) + "]";
}

int RobotW::getDistance() const {
	return abs(pos.getRow() - watson->getCurrentPosition().getRow()) +
	    		   abs(pos.getCol() - watson->getCurrentPosition().getCol());
}

void RobotW::move() {
	Position nextPos = getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
}

RobotType RobotW::getRobotType() const {
	return robot_type;
}

void RobotW::setItem(BaseItem* item){
	this->item = item;
}

BaseItem* RobotW::getItem() const{
	return item;
}

RobotSW::RobotSW(int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson)
	: Robot{index, init_pos, map, "RobotSW",SW}, criminal(criminal), sherlock(sherlock), watson(watson){}

RobotSW::~RobotSW(){
}

Position RobotSW::getNextPosition(){
	if (!map->isValid(pos, this) || !map->isValid(sherlock->getCurrentPosition(), sherlock) || !map->isValid(watson->getCurrentPosition(), watson)) {
		return Position::npos;
	} else {
		int min_adj_sherlock_watson_dist = 10000;
		Position bestPos = this->getCurrentPosition();
		for(int h = 0; h < 2; h++){
			Position adjacentPos[] = {
				bestPos.up(),
				bestPos.right(),
				bestPos.down(),
				bestPos.left()
			};

			for (const auto& adjPos : adjacentPos) {
				if(((adjPos.getRow() >= 0 && adjPos.getRow() < map->getRows())&& (adjPos.getCol() >= 0 && adjPos.getCol() < map->getCols()))){
					if(h == 1){
						if (map->isValid(adjPos, this)) {
							int adj_sherlock_dist = abs(adjPos.getRow() - sherlock->getCurrentPosition().getRow()) +
													abs(adjPos.getCol() - sherlock->getCurrentPosition().getCol());
							int adj_watson_dist = abs(adjPos.getRow() - watson->getCurrentPosition().getRow()) +
												  abs(adjPos.getCol() - watson->getCurrentPosition().getCol());

							if (adj_sherlock_dist + adj_watson_dist < min_adj_sherlock_watson_dist) {
								min_adj_sherlock_watson_dist = adj_sherlock_dist + adj_watson_dist;
								bestPos = adjPos;
							}
						}
					} else {
						int adj_sherlock_dist = abs(adjPos.getRow() - sherlock->getCurrentPosition().getRow()) +
												abs(adjPos.getCol() - sherlock->getCurrentPosition().getCol());
						int adj_watson_dist = abs(adjPos.getRow() - watson->getCurrentPosition().getRow()) +
											  abs(adjPos.getCol() - watson->getCurrentPosition().getCol());

						if (adj_sherlock_dist + adj_watson_dist < min_adj_sherlock_watson_dist) {
							min_adj_sherlock_watson_dist = adj_sherlock_dist + adj_watson_dist;
							bestPos = adjPos;
						}
					}
				}
			}
		}

		return bestPos;
	}
}

Position RobotSW::getCurrentPosition() const {
	return pos;
}

string RobotSW::str() const {
	return "Robot[pos=" + pos.str() + ";type=" + "SW"
			+ ";dist=" + to_string(getDistance()) + "]";
}

int RobotSW::getDistance() const {
	return  abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) +
	    			abs(pos.getCol() - sherlock->getCurrentPosition().getCol()) +
					abs(pos.getRow() - watson->getCurrentPosition().getRow()) +
					abs(pos.getCol() - watson->getCurrentPosition().getCol());
}

void RobotSW::move() {
	Position nextPos = getNextPosition();
	if (map->isValid(nextPos, this)) {
		pos = nextPos;
	}
}

RobotType RobotSW::getRobotType() const {
	return robot_type;
}

void RobotSW::setItem(BaseItem* item){
	this->item = item;
}

BaseItem* RobotSW::getItem() const{
	return item;
}

BaseItem::BaseItem(ItemType type) : type(type) {}

BaseItem::~BaseItem() {}

MagicBook::MagicBook(): BaseItem{MAGIC_BOOK}{}

bool MagicBook::canUse(Character* obj, Robot* robot) {
    if (obj->getExp() <= 350) {
        return true;
    }
    return false;
}

void MagicBook::use(Character* obj, Robot* robot) {
    int currentExp = obj->getExp();
    int increase = currentExp * 0.25;
    obj->setExp(currentExp + increase);
}

ItemType MagicBook::getType() {
    return type;
}

EnergyDrink::EnergyDrink(): BaseItem{ENERGY_DRINK}{}

bool EnergyDrink::canUse(Character* obj, Robot* robot) {
    if (obj->getHp() <= 100) {
        return true;
    }
    return false;
}

void EnergyDrink::use(Character* obj, Robot* robot) {
    int currentHp = obj->getHp();
    int increase = currentHp * 0.20;
    obj->setHp(currentHp + increase);
}

ItemType EnergyDrink::getType() {
    return type;
}

FirstAid::FirstAid(): BaseItem{FIRST_AID}{}

bool FirstAid::canUse(Character* obj, Robot* robot) {
    if (obj->getHp() <= 100 || obj->getExp() <= 350) {
        return true;
    }
    return false;
}

void FirstAid::use(Character* obj, Robot* robot) {
    int currentHp = obj->getHp();
    int increase = currentHp * 0.50;
    obj->setHp(currentHp + increase);
}

ItemType FirstAid::getType() {
    return type;
}

ExcemptionCard::ExcemptionCard(): BaseItem{EXCEMPTION_CARD}{}

bool ExcemptionCard::canUse(Character* obj, Robot* robot) {
    if (obj->getName() == "Sherlock" && obj->getHp() % 2 != 0 && robot != NULL) {
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character* obj, Robot* robot) {

}

ItemType ExcemptionCard::getType() {
    return type;
}

PassingCard::PassingCard(string challenge): BaseItem{PASSING_CARD}, challenge(challenge) {}

bool PassingCard::canUse(Character* obj, Robot* robot) {
	return true;
}

void PassingCard::use(Character* obj, Robot* robot) {
    if (to_string(robot->getRobotType()) != challenge){
    	if(challenge == "all"){
    	} else {
            int currentExp = obj->getExp();
            obj->setExp(currentExp - 50);
    	}
    }
    else if (to_string(robot->getRobotType()) == challenge){
    }
}

ItemType PassingCard::getType() {
    return type;
}

Node::~Node() {
}

BaseBag::BaseBag(Character* obj, int maxItems) : head(NULL), obj(obj), maxItems(maxItems) {}

BaseBag::~BaseBag() {
    while(head != NULL){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

bool BaseBag::insert(BaseItem* item) {
	if (getCount() >= maxItems) {
		return false;
	}

	Node* temp = new Node();
	temp->item = item;
	temp->next = head;
	head = temp;

	return true;
}

BaseItem* BaseBag::get() {
    if(this->getCount() == 0){
    	return NULL;
    }
	if (head == NULL) return NULL;

    Node* current = head;
    Node* prev = NULL;
    BaseItem* oldhead = head->item;
    while (current != NULL) {
        //if (current->item->getType() != PASSING_CARD && current->item->getType() != EXCEMPTION_CARD) {
    	if (current->item->canUse(obj, NULL)) {
            if (prev != NULL) {
                head->item = current->item;
                BaseItem* old_currentitem = current->item;
                current->item = oldhead;

                head = head->next;

                return old_currentitem;
            } else {
            	head = current->next;

                return current->item;
            }
        } else{
        	prev = current;
        	current = current->next;
        }

    }

    return NULL;
}

BaseItem* BaseBag::get(ItemType itemType) {
	if(this->getCount() == 0){
		return NULL;
	}
    if (head == NULL) return NULL;

    Node* current = head;
    Node* prev = NULL;
    BaseItem* oldhead = head->item;
    while (current != NULL) {
    	if (current->item->getType() == itemType) {

            if (prev != NULL) {
                head->item = current->item;
                BaseItem* old_currentitem = current->item;
                current->item = oldhead;

                head = head->next;

                return old_currentitem;
            } else {
                head = current->next;
                return current->item;
            }
        } else{
        	prev = current;
        	current = current->next;
        }

    }

    return NULL;
}



bool BaseBag::isAvailable(ItemType itemType) {
	if (head == NULL) return false;
	Node* current = head;
	while (current != NULL) {
		if (current->item->getType() == itemType) {
			return true;
		}
		current = current->next;
	}
	return false;
}

bool BaseBag::isFull() const{
	return this->getCount() >= maxItems;
}

string BaseBag::str() const {
	stringstream ss;
	ss << "Bag[count=" << getCount() << ";";
	Node* current = head;
	while (current != NULL) {
		ss << current->item->getType();
		if (current->next != NULL) {
			ss << ",";
		}
		current = current->next;
	}
	ss << "]";
	return ss.str();
}

int BaseBag::getCount() const {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

SherlockBag::SherlockBag(Sherlock* sherlock) : BaseBag{sherlock, 13} {}

WatsonBag::WatsonBag(Watson* watson) : BaseBag{watson, 15} {}

Configuration::Configuration(const string& filepath) {
	ifstream configFile(filepath);
	num_walls = 0;
	num_fake_walls = 0;
	string line;
	string attribute;
	string value;
    while (getline(configFile, line)) {
    	istringstream iss(line);
        getline(iss, attribute, '=');
		getline(iss, value);

		if (attribute == "NUM_WALLS") {
			num_walls = max(stoi(value),0);
		}
		else if (attribute == "NUM_FAKE_WALLS") {
			num_fake_walls = max(stoi(value),0);
		}
		else if (attribute == "MAP_NUM_ROWS") {
            map_num_rows = stoi(value);
        } else if (attribute == "MAP_NUM_COLS") {
            map_num_cols = stoi(value);
        } else if (attribute == "MAX_NUM_MOVING_OBJECTS") {
            max_num_moving_objects = stoi(value);
        } else if (attribute == "ARRAY_WALLS") {
        	int num_walls_updated = 0;
            for (char c : value) {
                if (c == ',') {
                	num_walls_updated++;
                }
            }
        	num_walls = max(num_walls_updated, num_walls);

            arr_walls = new Position[num_walls];
            for (int i = 0; i < num_walls; ++i) {
            	arr_walls[i] = Position(extractSubstring(value, i, num_walls));
			}
        } else if (attribute == "ARRAY_FAKE_WALLS") {
        	int num_fake_walls_updated = 0;
            for (char c : value) {
                if (c == ',') {
                	num_fake_walls_updated++;
                }
            }
            num_fake_walls = max(num_fake_walls_updated, num_fake_walls);
            arr_fake_walls = new Position[num_fake_walls];
            for (int i = 0; i < num_fake_walls; ++i) {
				arr_fake_walls[i] = Position(extractSubstring(value, i, num_fake_walls));
			}
        } else if (attribute == "SHERLOCK_MOVING_RULE") {
        	sherlock_moving_rule = value;
        } else if (attribute == "SHERLOCK_INIT_POS") {
        	sherlock_init_pos = Position(value);
        } else if (attribute == "SHERLOCK_INIT_HP") {
        	sherlock_init_hp = stoi(value);
        } else if (attribute == "SHERLOCK_INIT_EXP") {
        	sherlock_init_exp = stoi(value);
        } else if (attribute == "WATSON_MOVING_RULE") {
        	watson_moving_rule = value;
        } else if (attribute == "WATSON_INIT_POS") {
        	watson_init_pos = Position(value);
        } else if (attribute == "WATSON_INIT_HP") {
        	watson_init_hp = stoi(value);
        } else if (attribute == "WATSON_INIT_EXP") {
        	watson_init_exp = stoi(value);

        } else if (attribute == "CRIMINAL_INIT_POS") {
        	criminal_init_pos = Position(value);
        } else if (attribute == "NUM_STEPS") {
        	num_steps = stoi(value);
        }
    }
    configFile.close();
}

Configuration::~Configuration() {
    delete[] arr_walls;
    arr_walls = nullptr;
    delete[] arr_fake_walls;
    arr_fake_walls = nullptr;
}

string Configuration::str() const {
	string config_str = "Configuration[\n";
	        config_str += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
	        config_str += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
	        config_str += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
	        config_str += "NUM_WALLS=" + to_string(num_walls) + "\n";
	        config_str += "ARRAY_WALLS=[";
	        for(int i = 0; i < num_walls; i++){
	        	config_str += arr_walls[i].str();
	        	if (i < num_walls - 1){
	        		config_str += ";";
	        	}
	        }
	        config_str += "]\n";
	        config_str += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
	        config_str += "ARRAY_FAKE_WALLS=[";
	        for(int j = 0; j < num_fake_walls; j++){
				config_str += arr_fake_walls[j].str();
				if (j < num_fake_walls - 1){
					config_str += ";";
				}
			}
			config_str += "]\n";
	        config_str += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
	        config_str += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
	        config_str += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
	        config_str += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
	        config_str += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
	        config_str += "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
	        config_str += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
	        config_str += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
	        config_str += "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
	        config_str += "NUM_STEPS=" + to_string(num_steps) + "\n";
	        config_str += "]";
	        return config_str;
}

string Configuration::extractSubstring(const string& value, int index, int num) {
	int pos1 = value.find('[');
	int pos2 = value.find(';');
	if (static_cast<unsigned int>(pos2) == string::npos){
		pos2 = value.size() - 1;
	}
	if (index == 0) {
		return value.substr(pos1 + 1, pos2 - pos1 - 1);
	} else {
		int pos_last = value.find_last_of(';');
		int pos_next_last = value.find_last_of(']');
		if (index == num - 1) {
			return value.substr(pos_last + 1, pos_next_last - pos_last - 1);
		} else {
			int pos_prev = value.find(';', pos1 + 1);
			int pos_next = value.find(';', pos_prev + 1);
			for (int i = 1; i < index; ++i) {
				pos_prev = pos_next;
				pos_next = value.find(';', pos_prev + 1);
			}
			return value.substr(pos_prev + 1, pos_next - pos_prev - 1);
		}
	}
}

ArrayMovingObject::ArrayMovingObject(int capacity): capacity(capacity){
    arr_mv_objs = new MovingObject*[capacity];
}

ArrayMovingObject::~ArrayMovingObject(){
	count = 0;
}

bool ArrayMovingObject::isFull() const{
    return count >= capacity;
}

bool ArrayMovingObject::add(MovingObject * mv_obj){
    if(!isFull()){
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    } else{
        return false;
    }
}

MovingObject * ArrayMovingObject::get(int index) const{
    return arr_mv_objs[index];
}

string ArrayMovingObject::str() const{
    string result = "ArrayMovingObject[count=" + to_string(count) +
                                ";capacity=" + to_string(capacity) + ";";
    for(int i =0; i < count; i++){
        result += arr_mv_objs[i]->str();
        if (i < count -1) {
            result += ";";
        }
    }
    result += "]\n";
    return result;
}

int ArrayMovingObject::size(){
    return count;
}

StudyPinkProgram::StudyPinkProgram(const string & config_file_path){
	config = new Configuration(config_file_path);
	map = new Map(config->map_num_rows, config->map_num_cols,
				   config->num_walls, config->arr_walls,
				   config->num_fake_walls, config->arr_fake_walls);
	arrest =0;
	sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, max(min(config->sherlock_init_hp, 500), 0), max(min(config->sherlock_init_exp, 900), 0));
	watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, max(min(config->watson_init_hp, 500), 0), max(min(config->watson_init_exp, 900), 0));
	criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
	prev_criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
	arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
	arr_mv_objs->add(criminal);
	arr_mv_objs->add(sherlock);
	arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram(){
	 delete config;
	 delete sherlock;
	 delete watson;
	 delete criminal;
	 delete map;
	 delete arr_mv_objs;
}

bool StudyPinkProgram::isStop() const{
	return (((sherlock->getHp() == 0) && (watson->getHp() == 0)) ||
			(sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) ||
			(arrest == 1) ||
			(watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())));
}

void StudyPinkProgram::printResult() const {
	if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
		cout << "Sherlock caught the criminal" << endl;
	}
	else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
		cout << "Watson caught the criminal" << endl;
	}
	else {
		cout << "The criminal escaped" << endl;
	}
}

void StudyPinkProgram::printStep(int si) const {
	cout << "Step: " << setw(4) << setfill('0') << si
		<< "--"
		<< sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

int StudyPinkProgram::cardinal(int n) {
    if (n == 0){
        return 0;
    } else{
        int sum = n % 9;
        return (sum == 0) ? 9 : sum;
    }
}

void StudyPinkProgram::run(bool verbose) {
	int num_objects = 0;
    for (int istep = 0; istep < config->num_steps; ++istep) {
    	for (int i = 0; i < arr_mv_objs->size(); ++i) {
            if(arr_mv_objs->get(i)->getName() == "Criminal" &&
            		( arr_mv_objs->get(i)->getNextPosition().getRow() != Position::npos.getRow()
            		&& arr_mv_objs->get(i)->getNextPosition().getCol() != Position::npos.getCol())){
            	num_objects++;
            	if(num_objects == 3){
            		if(arr_mv_objs->size() == 3){
            			if(!arr_mv_objs->isFull()){
            				MovingObject * new_robotc;
            				int p = criminal->getCurrentPosition().getRow() * criminal->getCurrentPosition().getCol();
            				if(cardinal(p) >= 0 && cardinal(p) <= 1){
            					BaseItem * item = new MagicBook();

            					new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
            					new_robotc->setItem(item);

            				} else if(cardinal(p) >= 2 && cardinal(p) <= 3){
            					BaseItem * item = new EnergyDrink();

            					new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
								new_robotc->setItem(item);


            				} else if(cardinal(p) >= 4 && cardinal(p) <= 5){
            					BaseItem * item = new FirstAid();

            					new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
								new_robotc->setItem(item);

            				} else if(cardinal(p) >= 6 && cardinal(p) <= 7){
            					BaseItem * item = new ExcemptionCard();

            					new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
								new_robotc->setItem(item);

            				} else if(cardinal(p) >= 8 && cardinal(p) <= 9){
            					int t = (criminal->getCurrentPosition().getRow() * 11 + criminal->getCurrentPosition().getCol()) % 4;

            					if (t == 0){
            						BaseItem * item = new PassingCard("RobotS");

            						new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
									new_robotc->setItem(item);
            					} else if (t == 1){
            						BaseItem * item = new PassingCard("RobotC");

            						new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
									new_robotc->setItem(item);
            					} else if (t == 2){
            						BaseItem * item = new PassingCard("RobotSW");

            						new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
									new_robotc->setItem(item);
            					} else if (t == 3){
            						BaseItem * item = new PassingCard("all");

            						new_robotc = new RobotC(i, criminal->getCurrentPosition(), map, prev_criminal);
									new_robotc->setItem(item);
            					}

            				}
							arr_mv_objs->add(new_robotc);
            			}

            		} else if (arr_mv_objs->size() > 3){
						if(!arr_mv_objs->isFull()){
							MovingObject * new_robot;
							int sherlock_dist = abs(criminal->getCurrentPosition().getRow() - sherlock->getCurrentPosition().getRow()) +
												abs(criminal->getCurrentPosition().getCol() - sherlock->getCurrentPosition().getCol());
							int watson_dist = abs(criminal->getCurrentPosition().getRow() - watson->getCurrentPosition().getRow()) +
											  abs(criminal->getCurrentPosition().getCol() - watson->getCurrentPosition().getCol());
							if (sherlock_dist < watson_dist){
								int p = criminal->getCurrentPosition().getRow() * criminal->getCurrentPosition().getCol();
								if(cardinal(p) >= 0 && cardinal(p) <= 1){
									BaseItem * item = new MagicBook();

									new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

									new_robot->setItem(item);
								} else if(cardinal(p) >= 2 && cardinal(p) <= 3){
									BaseItem * item = new EnergyDrink();


									new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 4 && cardinal(p) <= 5){
									BaseItem * item = new FirstAid();

									new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 6 && cardinal(p) <= 7){
									BaseItem * item = new ExcemptionCard();

									new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 8 && cardinal(p) <= 9){
									int t = (criminal->getCurrentPosition().getRow() * 11 + criminal->getCurrentPosition().getCol()) % 4;

									if (t == 0){
										BaseItem * item = new PassingCard("RobotS");
										new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

										new_robot->setItem(item);

									} else if (t == 1){
										BaseItem * item = new PassingCard("RobotC");

										new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

										new_robot->setItem(item);
									} else if (t == 2){
										BaseItem * item = new PassingCard("RobotSW");

										new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

										new_robot->setItem(item);
									} else if (t == 3){
										BaseItem * item = new PassingCard("all");

										new_robot = new RobotS(i, criminal->getCurrentPosition(), map, criminal, sherlock);

										new_robot->setItem(item);
									}

								}
								arr_mv_objs->add(new_robot);

							} else if (watson_dist < sherlock_dist){
								int p = criminal->getCurrentPosition().getRow() * criminal->getCurrentPosition().getCol();
								if(cardinal(p) >= 0 && cardinal(p) <= 1){
									BaseItem * item = new MagicBook();

									new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

									new_robot->setItem(item);
								} else if(cardinal(p) >= 2 && cardinal(p) <= 3){
									BaseItem * item = new EnergyDrink();

									new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 4 && cardinal(p) <= 5){
									BaseItem * item = new FirstAid();

									new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

									new_robot->setItem(item);


								} else if(cardinal(p) >= 6 && cardinal(p) <= 7){
									BaseItem * item = new ExcemptionCard();

									new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 8 && cardinal(p) <= 9){
									int t = (criminal->getCurrentPosition().getRow() * 11 + criminal->getCurrentPosition().getCol()) % 4;

									if (t == 0){
										BaseItem * item = new PassingCard("RobotS");
										new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

										new_robot->setItem(item);

									} else if (t == 1){
										BaseItem * item = new PassingCard("RobotC");

										new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

										new_robot->setItem(item);
									} else if (t == 2){
										BaseItem * item = new PassingCard("RobotSW");

										new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

										new_robot->setItem(item);
									} else if (t == 3){
										BaseItem * item = new PassingCard("all");

										new_robot = new RobotW(i, criminal->getCurrentPosition(), map, criminal, watson);

										new_robot->setItem(item);
									}
								}
								arr_mv_objs->add(new_robot);

							} else if (watson_dist == sherlock_dist){
								int p = criminal->getCurrentPosition().getRow() * criminal->getCurrentPosition().getCol();
								if(cardinal(p) >= 0 && cardinal(p) <= 1){
									BaseItem * item = new MagicBook();

									new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 2 && cardinal(p) <= 3){
									BaseItem * item = new EnergyDrink();

									new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

									new_robot->setItem(item);


								} else if(cardinal(p) >= 4 && cardinal(p) <= 5){
									BaseItem * item = new FirstAid();

									new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

									new_robot->setItem(item);


								} else if(cardinal(p) >= 6 && cardinal(p) <= 7){
									BaseItem * item = new ExcemptionCard();

									new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

									new_robot->setItem(item);

								} else if(cardinal(p) >= 8 && cardinal(p) <= 9){
									int t = (criminal->getCurrentPosition().getRow() * 11 + criminal->getCurrentPosition().getCol()) % 4;
									if (t == 0){
										BaseItem * item = new PassingCard("RobotS");
										new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

										new_robot->setItem(item);

									} else if (t == 1){

										BaseItem * item = new PassingCard("RobotC");
										new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

										new_robot->setItem(item);
									} else if (t == 2){
										BaseItem * item = new PassingCard("RobotSW");

										new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

										new_robot->setItem(item);
									} else if (t == 3){
										BaseItem * item = new PassingCard("all");

										new_robot = new RobotSW(i, criminal->getCurrentPosition(), map, criminal, sherlock, watson);

										new_robot->setItem(item);
									}
								}
								arr_mv_objs->add(new_robot);
							}
						}
            		}
            		num_objects = 0;
            	}
            }


            if(arr_mv_objs->get(i)->getNextPosition().getRow() != Position::npos.getRow()
            		&& arr_mv_objs->get(i)->getNextPosition().getCol() != Position::npos.getCol()){
            	if(arr_mv_objs->get(i)->getName() == "Criminal" && istep > 0){
            		prev_criminal->move();
				}

            	arr_mv_objs->get(i)->move();

            } else{
            	if (arr_mv_objs->get(i)->getName() == "Sherlock"){
            		Sherlock::ruleIndex = (Sherlock::ruleIndex + 1) % arr_mv_objs->get(i)->getSizeMovingRule();
            	} else if (arr_mv_objs->get(i)->getName() == "Watson"){
            		Watson::ruleIndex = (Watson::ruleIndex + 1) % arr_mv_objs->get(i)->getSizeMovingRule();
            	}
            }


            //Sherlock situations:
            if(arr_mv_objs->get(i)->getName() == "Sherlock"){

            	for (int t = 0; t < arr_mv_objs->size(); ++t) {

            		if( t == i){
            			continue;
            		} else{
            			if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(arr_mv_objs->get(t)->getCurrentPosition())){
            				// Sherlock meets Watson:
            				if(arr_mv_objs->get(t)->getName() == "Watson"){
            					while(sherlock->getBag()->isAvailable(PASSING_CARD) && watson->getBag()->isAvailable(EXCEMPTION_CARD)){
									if(watson->getBag()->isFull() || sherlock->getBag()->isFull()){
										break;
									}
									watson->getBag()->insert(sherlock->useItem(PASSING_CARD));
									sherlock->getBag()->insert(watson->useItem(EXCEMPTION_CARD));
								}
            				}
            				// Sherlock meets RobotS:
            				else if (arr_mv_objs->get(t)->getName() =="RobotS" ){
            					MovingObject* obj_robot = new RobotS(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock);
								RobotS* robots = dynamic_cast<RobotS*>(obj_robot);
								robots->setItem(arr_mv_objs->get(t)->getItem());
								// first time item
								if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
									BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
									s_item1->use(sherlock, robots);
									if (sherlock->getExp() > 400){
										sherlock->addItem(robots->getItem());
									}
								} else {
									if (sherlock->getExp() > 400){
										sherlock->addItem(robots->getItem());
									} else {
                        				sherlock->setExp(sherlock->getExp()*0.9);
                        			}
								}
                    			// second time item
                    			BaseItem * s_item2 = sherlock->getBag()->get();
                    			if(s_item2 != NULL){
                    				s_item2->use(sherlock, robots);
                    			}

            				}
            				// Sherlock meets RobotW:
            				else if (arr_mv_objs->get(t)->getName() =="RobotW"){
            					MovingObject* obj_robot = new RobotW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, watson);
								RobotW* robotw = dynamic_cast<RobotW*>(obj_robot);
								robotw->setItem(arr_mv_objs->get(t)->getItem());
								// first time item
								if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
									BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
									s_item1->use(sherlock, robotw);
								}

								sherlock->addItem(robotw->getItem());

								// second time item
								BaseItem * s_item2 = sherlock->getBag()->get();
								if(s_item2 != NULL){
									s_item2->use(sherlock, robotw);
								}

            				}
            				// Sherlock meets RobotSW:
            				else if (arr_mv_objs->get(t)->getName() =="RobotSW"){
            					MovingObject* obj_robot = new RobotSW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock, watson);
            					RobotSW* robotsw = dynamic_cast<RobotSW*>(obj_robot);
            					robotsw->setItem(arr_mv_objs->get(t)->getItem());
            					// first time item
								if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
									BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
									s_item1->use(sherlock, robotsw);
									if (sherlock->getExp() > 300 && sherlock->getHp() > 335){
										sherlock->addItem(robotsw->getItem());
									}
								} else {
									if (sherlock->getExp() > 300 && sherlock->getHp() > 335){
										sherlock->addItem(robotsw->getItem());
									} else {
										sherlock->setExp(sherlock->getExp()*0.9);
									}
								}
								// second time item
								BaseItem * s_item2 = sherlock->getBag()->get();
								if(s_item2 != NULL){
									s_item2->use(sherlock, robotsw);
								}

            				}
							// Sherlock meets RobotC:
            				else if (arr_mv_objs->get(t)->getName() =="RobotC"){
            					MovingObject* obj_robot = new RobotC(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal);
								RobotC* robotc = dynamic_cast<RobotC*>(obj_robot);
								robotc->setItem(arr_mv_objs->get(t)->getItem());
								// first time item
								if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
									BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
									s_item1->use(sherlock, robotc);
								}

								if(sherlock->getExp() > 500){
									arrest = 1;
								} else {
									sherlock->addItem(robotc->getItem());
								}

								// second time item
								BaseItem * s_item2 = sherlock->getBag()->get();
								if(s_item2 != NULL){
									s_item2->use(sherlock, robotc);
								}

            				}
            			}
            		}
            	}

            }
            //Watson situations:
            if(arr_mv_objs->get(i)->getName() == "Watson"){
            	for (int t = 0; t < arr_mv_objs->size(); ++t) {

					if( t == i){
						continue;
					} else{
						if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(arr_mv_objs->get(t)->getCurrentPosition())){
							// Watson meets Sherlock:
							if(arr_mv_objs->get(t)->getName() == "Sherlock"){
								while(sherlock->getBag()->isAvailable(PASSING_CARD) && watson->getBag()->isAvailable(EXCEMPTION_CARD)){
									if(watson->getBag()->isFull() || sherlock->getBag()->isFull()){
										break;
									}
									watson->getBag()->insert(sherlock->useItem(PASSING_CARD));
									sherlock->getBag()->insert(watson->useItem(EXCEMPTION_CARD));
								}
							}
							// Watson meets RobotS:
							else if (arr_mv_objs->get(t)->getName() =="RobotS" ){
								MovingObject* obj_robot = new RobotS(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock);
								RobotS* robots = dynamic_cast<RobotS*>(obj_robot);
								robots->setItem(arr_mv_objs->get(t)->getItem());
								//first time item
								if(watson->getBag()->isAvailable(PASSING_CARD)){
									BaseItem * w_item1 = watson->useItem(PASSING_CARD);
									w_item1->use(watson, robots);
								}

								// do nothing

								// second time item
								BaseItem * w_item2 = watson->getBag()->get();
								if(w_item2 != NULL){
									w_item2->use(watson, robots);
								}
							}
							// Watson meets RobotW:
							else if (arr_mv_objs->get(t)->getName() =="RobotW"){
								MovingObject* obj_robot = new RobotW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, watson);
								RobotW* robotw = dynamic_cast<RobotW*>(obj_robot);
								robotw->setItem(arr_mv_objs->get(t)->getItem());
								//first time item
								if(watson->getBag()->isAvailable(PASSING_CARD)){
									BaseItem * w_item1 = watson->useItem(PASSING_CARD);
									w_item1->use(watson, robotw);
									watson->addItem(robotw->getItem());
								} else{
									if (watson->getHp() > 350){
										watson->addItem(robotw->getItem());
									} else{
										watson->setHp(watson->getHp()*0.95);
									}

								}
								//second time item
								BaseItem * w_item2 = watson->getBag()->get();
								if(w_item2 != NULL){
									w_item2->use(watson, robotw);
								}

							}
							// Watson meets RobotSW:
							else if (arr_mv_objs->get(t)->getName() =="RobotSW"){
								MovingObject* obj_robot = new RobotSW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock, watson);
								RobotSW* robotsw = dynamic_cast<RobotSW*>(obj_robot);
								robotsw->setItem(arr_mv_objs->get(t)->getItem());
								//first time item
								if(watson->getBag()->isAvailable(PASSING_CARD)){
									BaseItem * w_item1 = watson->useItem(PASSING_CARD);
									w_item1->use(watson, robotsw);
									watson->addItem(robotsw->getItem());
								} else{
									if (watson->getExp() > 600 && watson->getHp() > 165){
										watson->addItem(robotsw->getItem());
									} else{
										watson->setHp(watson->getHp()*0.85);
										watson->setExp(watson->getExp()*0.85);
									}

								}
								//second time item
								BaseItem * w_item2 = watson->getBag()->get();
								if(w_item2 != NULL){
									w_item2->use(watson, robotsw);
								}

							}

							// Watson meets RobotC:
							else if (arr_mv_objs->get(t)->getName() =="RobotC"){
								MovingObject* obj_robot = new RobotC(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal);
								RobotC* robotc = dynamic_cast<RobotC*>(obj_robot);
								robotc->setItem(arr_mv_objs->get(t)->getItem());
								// first time item
								if(watson->getBag()->isAvailable(PASSING_CARD)){
									BaseItem * w_item1 = watson->useItem(PASSING_CARD);
									w_item1->use(watson, robotc);
								}

								watson->addItem(robotc->getItem());

								// second time item
								BaseItem * w_item2 = watson->getBag()->get();
								if(w_item2 != NULL){
									w_item2->use(watson, robotc);
								}

							}
						}
					}
            	}
            }
        	//RobotS situations
        	if(arr_mv_objs->get(i)->getName() == "RobotS"){
        		MovingObject* obj_robot = new RobotS(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock);
				RobotS* robots = dynamic_cast<RobotS*>(obj_robot);
				robots->setItem(arr_mv_objs->get(i)->getItem());
        		//RobotS meets Sherlock
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition())){
        			// first time item
					if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
						BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
						s_item1->use(sherlock, robots);
						if (sherlock->getExp() > 400){
							sherlock->addItem(robots->getItem());
						}
					} else {
						if (sherlock->getExp() > 400){
							sherlock->addItem(robots->getItem());
						} else {
							sherlock->setExp(sherlock->getExp()*0.9);
						}
					}

        			// second time item
        			BaseItem * s_item2 = sherlock->getBag()->get();
        			if(s_item2 != NULL){
        				s_item2->use(sherlock, robots);
					}

        		}
        		//RobotS meets Watson
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition())){

        			// first time item
        			if(watson->getBag()->isAvailable(PASSING_CARD)){
						// co passing card
						BaseItem * w_item1 = watson->useItem(PASSING_CARD);
						w_item1->use(watson, robots);
					}

        			// do nothing

        			// second time item
        			BaseItem * w_item2 = watson->getBag()->get();
        			if(w_item2 != NULL){
        				w_item2->use(watson, robots);
					}

        		}
        	}
        	// RobotW situations
        	if(arr_mv_objs->get(i)->getName() == "RobotW"){
        		MovingObject* obj_robot = new RobotW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, watson);
				RobotW* robotw = dynamic_cast<RobotW*>(obj_robot);
				robotw->setItem(arr_mv_objs->get(i)->getItem());
        		//RobotW meets Sherlock
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition())){
        			// first time item
					if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
						BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
						s_item1->use(sherlock, robotw);
					}

        			sherlock->addItem(robotw->getItem());

        			// second time item
        			BaseItem * s_item2 = sherlock->getBag()->get();
        			if(s_item2 != NULL){
        				s_item2->use(sherlock, robotw);
					}

        		}
        		//RobotW meets Watson
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition())){
        			// first time item
        			if(watson->getBag()->isAvailable(PASSING_CARD)){
						BaseItem * w_item1 = watson->useItem(PASSING_CARD);
						w_item1->use(watson, robotw);
						watson->addItem(robotw->getItem());
					} else{
						if (watson->getHp() > 350){
							watson->addItem(robotw->getItem());
						} else{
							watson->setHp(watson->getHp()*0.95);
						}
					}
        			//second time item
        			if (watson->getBag()->getCount() != 0){
        				BaseItem * w_item2 = watson->getBag()->get();
						if(w_item2 != NULL){
							w_item2->use(watson, robotw);
						}
        			}
        		}
        	}
        	// RobotSW situations
        	if(arr_mv_objs->get(i)->getName() == "RobotSW"){
        		MovingObject* obj_robot = new RobotSW(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal, sherlock, watson);
        		RobotSW* robotsw = dynamic_cast<RobotSW*>(obj_robot);
        		robotsw->setItem(arr_mv_objs->get(i)->getItem());
        		//RobotSW meets Sherlock
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition())){
        			// first time item
					if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
						BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
						s_item1->use(sherlock, robotsw);
						if (sherlock->getExp() > 300 && sherlock->getHp() > 335){
							sherlock->addItem(robotsw->getItem());
						}
					} else {
						if (sherlock->getExp() > 300 && sherlock->getHp() > 335){
							sherlock->addItem(robotsw->getItem());
						} else {
							sherlock->setExp(sherlock->getExp()*0.9);
						}
					}
						// second time item
						BaseItem * s_item2 = sherlock->getBag()->get();
						if(s_item2 != NULL){
							s_item2->use(sherlock, robotsw);
						}

				}

        		//RobotSW meets Watson
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition())){
        			// first time item
        			if(watson->getBag()->isAvailable(PASSING_CARD)){
						BaseItem * w_item1 = watson->useItem(PASSING_CARD);
						w_item1->use(watson, robotsw);
						watson->addItem(robotsw->getItem());
					} else{
						if (watson->getExp() > 600 && watson->getHp() > 165){
							watson->addItem(robotsw->getItem());
						} else{
							watson->setHp(watson->getHp()*0.85);
							watson->setExp(watson->getExp()*0.85);
						}
					}
        			//second time item
        			BaseItem * w_item2 = watson->getBag()->get();
        			if(w_item2 != NULL){
        				w_item2->use(watson, robotsw);
					}

        		}
        	}
        	// RobotC situations
        	if(arr_mv_objs->get(i)->getName() == "RobotC"){
        		MovingObject* obj_robot = new RobotC(i, arr_mv_objs->get(i)->getCurrentPosition(), map , criminal);
        		RobotC* robotc = dynamic_cast<RobotC*>(obj_robot);
        		robotc->setItem(arr_mv_objs->get(i)->getItem());
        		//RobotC meets Sherlock
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(sherlock->getCurrentPosition())){
        			// first time item
					if(sherlock->getBag()->isAvailable(EXCEMPTION_CARD)){
						BaseItem * s_item1 = sherlock->useItem(EXCEMPTION_CARD);
						s_item1->use(sherlock, robotc);
					}

        			if(sherlock->getExp() > 500){
						arrest = 1;
					} else {
						sherlock->addItem(robotc->getItem());
					}

        			// second time item
        			BaseItem * s_item2 = sherlock->getBag()->get();
        			if(s_item2 != NULL){
        				s_item2->use(sherlock, robotc);
					}

        		}
        		//RobotC meets Watson
        		if(arr_mv_objs->get(i)->getCurrentPosition().isEqual(watson->getCurrentPosition())){
        			// first time item
        			if(watson->getBag()->isAvailable(PASSING_CARD)){
						BaseItem * w_item1 = watson->useItem(PASSING_CARD);
						w_item1->use(watson, robotc);
					}

        			watson->addItem(robotc->getItem());

        			// second time item
        			BaseItem * w_item2 = watson->getBag()->get();
        			if(w_item2 != NULL){
        				w_item2->use(watson, robotc);
        			}
        		}
        	}
        	if (verbose) {
        		printStep(istep);
        	}
            if (isStop()) {
                break;
            }

        }
        if (isStop()) {
        	break;
		}
    }
    printResult();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
