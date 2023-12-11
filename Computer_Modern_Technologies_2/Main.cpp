#include "MagnetDirectTask.h"
#include "Reciver.h"
int main()
{
	std::vector<MagnetElement<2, 2>> elems;
	MagnetDirectTask<2,2> directTask(elems);
	Reciver<2, 2> rec;
	directTask.getIndoctValue<1>(rec.point, 5);
	return 0;
}