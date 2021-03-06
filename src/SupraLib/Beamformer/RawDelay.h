// ================================================================================================
// 
// If not explicitly stated: Copyright (C) 2016, all rights reserved,
//      Rüdiger Göbl 
//		Email r.goebl@tum.de
//      Chair for Computer Aided Medical Procedures
//      Technische Universität München
//      Boltzmannstr. 3, 85748 Garching b. München, Germany
// 
// ================================================================================================

#ifndef __RAWDELAY_H__
#define __RAWDELAY_H__

#include "USImage.h"
#include "WindowFunction.h"
#include "RxBeamformerParameters.h"

#include <memory>

namespace supra
{
	struct ScanlineRxParameters3D;
	class USRawData;

	using std::shared_ptr;

	class RawDelay
	{
	public:
		enum RxSampleBeamformer {
			DelayAndSum,
			DelayAndStdDev,
			TestSignal,
			INVALID
		};

		RawDelay(const RxBeamformerParameters& parameters);
		~RawDelay();

		// apply the receive delay
		template <typename ChannelDataType, typename OutputType>
		shared_ptr<USRawData> performDelay(
			shared_ptr<const USRawData> rawData,
			double fNumber,
			double speedOfSoundMMperS,
			WindowType windowType,
			WindowFunction::ElementType windowParameters) const;
		
	private:
		typedef RxBeamformerParameters::LocationType LocationType;

		void convertToDtSpace(double dt, double speedOfSoundMMperS, size_t numTransducerElements) const;

		// Imaging parameters
		size_t m_numRxScanlines;
		vec2s m_rxScanlineLayout;
		LocationType m_depth;

		// prepared Rx parameters
		mutable std::unique_ptr<Container<LocationType> > m_pRxDepths;
		mutable std::unique_ptr<Container<ScanlineRxParameters3D> > m_pRxScanlines;
		mutable std::unique_ptr<Container<LocationType> > m_pRxElementXs;
		mutable std::unique_ptr<Container<LocationType> > m_pRxElementYs;
		size_t m_rxNumDepths;

		mutable double m_speedOfSoundMMperS;
		mutable double m_lastSeenDt;
		mutable shared_ptr<const USImageProperties> m_lastSeenImageProperties;
		mutable shared_ptr<const USImageProperties> m_editedImageProperties;

		bool m_is3D;

		static constexpr size_t m_windowFunctionNumEntries = 64;
		mutable std::unique_ptr<WindowFunction> m_windowFunction;
	};
}

#endif //!__RAWDELAY_H__
