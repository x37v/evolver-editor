#ifndef APPLICATION_MODEL_HPP
#define APPLICATION_MODEL_HPP

#include <QObject>
#include <vector>

class LFOModel;
class AnalogOscModel;
class DigitalOscModel;

class ApplicationModel : public QObject {
	Q_OBJECT
	public:
		ApplicationModel(QObject * parent = NULL);
	private:
		std::vector<AnalogOscModel *> mAnalogOscs;
		std::vector<DigitalOscModel *> mDigitalOscs;
		std::vector<LFOModel *> mLFOs;
};

#endif
