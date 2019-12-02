#include "serviceBase.hpp"
using namespace std::chrono;
ServiceBase::ServiceBase(){};
ServiceBase::~ServiceBase(){};

// nxt = 0
// while system_running:
//     # print(str(self.getIdentifier()) + " :: " + "called fixFingers")
//     self.log("fixFingers")
//     nxt = nxt + 1
//     if nxt > NBITS:
//         # self.printFingerable()
//         nxt = 1
//     self._finger[nxt - 1] = self.findSuccessor(self.getIdentifier(1 << (nxt - 1)))

//     time.sleep(SLEEP_TIME)

void ServiceBase::fixFingers() {
	// TODO need to chamnge stopping mechanism of this while loop
	bool running = true;
	int nxt = 0;
	while(running) {
		nxt++;
		// TODO place NBITS in proper location
		if(nxt > NBITS) {
			nxt = 1;
		}
		// TODO write operator overload for [] op
		// overload the [] operator to mask the Map object
		m_table->Map[nxt - 1] = findSuccessor();
		//m_table[nxt - 1] = 
		// TODO make inclide.hpp header file for all such such function
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

// def closestPrecedingNode(self, id):
//     # check paper for implementation
//     self.log("closestPrecedingNode")
//     for idx in reversed(range(NBITS)):
//         if self._finger[idx] != None and \
//                 (inrange(self._finger[idx].getIdentifier(), self.getIdentifier(), id) and \
//                  (self.getIdentifier() != id) and \
//                  (self._finger[idx].getIdentifier() != self.getIdentifier()) and \
//                  (self._finger[idx].getIdentifier() != id)):
//             return self._finger[idx]

//     return self

NodeBase* ServiceBase::closestPrecedingNode(unsigned int Id) {
	for(int idx = NBITS - 1;idx >=0 ;idx--) {
		if(m_table->Map[idx]->getId() != -1) 
	}
}
