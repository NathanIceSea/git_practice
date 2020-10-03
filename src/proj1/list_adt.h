// 该头文件提供 链表ADT 的定义，应由实现头文件包含
// 为了使用该头文件，实现头文件必须定义 _USE_LIST_ADT_H_
// 实现头文件必须提供恰当的 List、Position、ElementType 类型声明
// 一个编译单元中最多使用一种实现


#ifdef _USE_LIST_ADT_H_
#undef _USE_LIST_ADT_H_

List MakeEmpty();
int IsEmpty(List li);
int IsLast(Position pos, List li);
ElementType Get(Position pos, List li);
Position Advance(Position curr, List li);
Position Find(ElementType e, List li);
Position FindPrevious(ElementType e, List li);
void Delete(Position pos, List li);
void DeleteElem(ElementType e, List li);
void Insert(Position pos, ElementType e, List li);
void DeleteList(List li);
Position Header(List li);
Position First(List li);
Position Last(List li);

#endif