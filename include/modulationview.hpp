#ifndef MODULATION_VIEW_HPP
#define MODULATION_VIEW_HPP

#include <QWidget>
#include <vector>

class LFOView;
class ModRoutingView;
class MiscModulationView;
//this shows all the modulation controls on the evolver
class ModulationView : public QWidget {
	Q_OBJECT
	public:
		ModulationView(QWidget * parent = NULL);
	private:
		std::vector<LFOView *> mLFOs;
		std::vector<ModRoutingView *> mMods;
		MiscModulationView * mMiscModulation;
};

#endif
