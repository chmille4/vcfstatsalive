#include "BasicStatsCollector.h"
#include "StandardDeviationChangeMonitor.h"
#include "DeltaAverageRatioChangeMonitor.h"

using namespace BamstatsAlive;

static const unsigned int kCMTrailLength = 5;
static const double kCMThreshold = 0.001;

BasicStatsCollector::BasicStatsCollector() {

	_stats.clear();

	_stats[kTotalRecords] = 0;
	_stats[kTsTvRatio] = 0;


#ifdef DEBUG
	StatMapT::iterator iter;
	for(iter = _stats.begin(); iter != _stats.end(); iter++) {
		std::cerr<<"Initializing: "<<iter->first<<std::endl;
	}
#endif

	_monitors.clear();
	_monitors[kTotalRecords] 		= new StandardDeviationChangeMonitor<double>(kCMTrailLength, kCMThreshold);
	_monitors[kTsTvRatio] 			= new StandardDeviationChangeMonitor<double>(kCMTrailLength, kCMThreshold);
}

void BasicStatsCollector::processVariantImpl(const vcf::Variant& var) {
	// increment total variant counter
	++_stats[kTotalRecords];

	assert(var.ref.length() > 0);


	if(var.ref.length() == 1) {
		for(size_t i=0; i<var.alt.size(); i++) {
			assert(var.alt[i].length > 0);
			
		}
	}

	
}

void BasicStatsCollector::appendJsonImpl(json_t * jsonRootObj) {
	StatMapT::iterator sIter;
	for(sIter = _stats.begin(); sIter != _stats.end(); sIter++) {
		json_object_set_new(jsonRootObj, sIter->first.c_str(), json_integer(sIter->second));
	}

	ChangeMonitorMapT::iterator cIter;
	for(cIter = _monitors.begin(); cIter != _monitors.end(); cIter++) {
		dynamic_cast<StandardDeviationChangeMonitor<double> *>(cIter->second)->addValue(_stats[cIter->first] / static_cast<double>(_stats[kTotalReads]));
	}

	bool consensus = true;
}