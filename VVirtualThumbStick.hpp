/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2014 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

/// \file VVirtualThumbStick.hpp

#ifndef V_RTSVVIRTUALTHUMBSTICK_HPP_INCLUDED
#define V_RTSVVIRTUALTHUMBSTICK_HPP_INCLUDED

#ifdef SUPPORTS_MULTITOUCH

namespace Rts {

/// \brief
///   Input class that implements a virtual stumb stick for touch devices.
class VVirtualThumbStick : public IVInputDevice, public IVisCallbackHandler_cl
{
public:
  /// \brief
  ///   Constructor
  ///
  /// \param validArea
  ///   Defines the area in pixel coordinates where the virtual thumb stick is accessible (optional).
  ///   If an invalid rectangle is given the default rectangle is used.
	///	\param fPriority
	///		The priority of the touch area for this thumbstick, higher values take priority over lower values
	///	\param bRightStick
	///		true to have the virtual stick emualte a right stick, false to emulate a left stick
	/// \param fRelativeInitialX
  ///   Initial x-position of the thumb stick relative to the validArea.
  /// \param fRelativeInitialY
  ///   Initial y-position of the thumb stick relative to the validArea.
  /// \param szCirclePath
  ///   File path to the inner circle texture (optional).
  /// \param szRingPath
  ///   File path to the outer ring texture (optional).
  VVirtualThumbStick(const VRectanglef& validArea = VRectanglef(),
    float fRelativeInitialX = 0.5f, float fRelativeInitialY = 0.5f,
		float fPriority = -1500.0f, bool bRightStick = false, 
    const char* szCirclePath = NULL, const char* szRingPath = NULL);

  /// \brief
  ///   Destructor
  virtual ~VVirtualThumbStick();
  
  /// \brief
  ///   Shows the virtual thumb stick control.
  ///
  /// \param bShow
  ///   Set to false if the control should not be shown.
  void Show(bool bShow = true);

  /// \brief
  ///   Hides the virtual thumb stick control.
  void Hide();
  
  virtual void Reset() HKV_OVERRIDE;

  virtual bool IsActive() HKV_OVERRIDE;

  virtual int GetRawControlValue(unsigned int uiControl) HKV_OVERRIDE;

  /// \brief
  ///   Returns the control value of a specific control.
  ///
  /// \param uiControl
  ///   For left stick can be: CT_PAD_LEFT_THUMB_STICK_UP, CT_PAD_LEFT_THUMB_STICK_DOWN, 
  ///   CT_PAD_LEFT_THUMB_STICK_LEFT or CT_PAD_LEFT_THUMB_STICK_RIGHT.
	///   For right stick can be: CT_PAD_RIGHT_THUMB_STICK_UP, CT_PAD_RIGHT_THUMB_STICK_DOWN, 
  ///   CT_PAD_RIGHT_THUMB_STICK_LEFT or CT_PAD_RIGHT_THUMB_STICK_RIGHT.	
	/// \param fDeadZone
  ///   The minimum value for which input is detected.
  /// \param bTimeScaled
  ///   Determines if control value is scaled by the frame duration.
  virtual float GetControlValue(unsigned int uiControl, float fDeadZone, bool bTimeScaled = false) HKV_OVERRIDE;
  
  virtual const char* GetName() HKV_OVERRIDE;

  virtual int GetModel() HKV_OVERRIDE
  {
    return INPUT_DEVICE_EMULATED_JOYSTICK;
  }

  /// \brief
  ///   Returns the area in which the virtual thumb stick can be used.
  inline const VRectanglef GetValidArea() const
  {
    return m_validArea;
  }

  /// \brief
  ///   Sets the are in which the virtual thumb stick can be used.
  ///
  // \param validArea
  ///   Defines the area in pixel coordinates where the virtual thumb stick is accessible.
  ///   If an invalid rectangle is given the default rectangle is used.
  void SetValidArea(const VRectanglef& validArea);

  /// \brief
  ///   Callback handler implementation.
  virtual void OnHandleCallback(IVisCallbackDataObject_cl *pData) HKV_OVERRIDE;

	/// \brief
	/// Factors for the size of the touch area after a touch has been recognised.
	/// These values scale the size of the touch area, which would normally be the 
	/// same size as m_spRingMask image. value of 1.0 gives the same size area as the ring mask.
	/// \param fXFactor
	///		factor for the width of the touch area after a touch has been recognised (1.0f for the same width as the ring mask image)
	///		must be > 0
	/// \param fYFactor
	///		factor for the height of the touch area after a touch ahs been recognised (1.0f for the same height as the ring mask image)
	///		must be > 0
	inline virtual void SetAfterTouchSize( float fXFactor = 1.0f, float fYFactor = 1.0f ) 
	{
		VASSERT_MSG( fXFactor > 0.0f, "VVirtualThumbStick::setAfterTouchSize - The after touch x size must be > 0" );
		VASSERT_MSG( fYFactor > 0.0f, "VVirtualThumbStick::setAfterTouchSize - The after touch y size must be > 0" );
		m_fAfterTouchSizeX = fXFactor;
		m_fAfterTouchSizeY = fYFactor;
	}

	/// \brief
	///	Sets the color used for the thumbstick images when it is actively in use
	inline virtual void SetActiveColor( VColorRef& color )  
	{
		m_colorActive.SetRGBA( color.r, color.g, color.b, color.a );
	}
	
	/// \brief
	///	Sets the color used for the thumbstick images when it is not actively in use
	inline virtual void SetInActiveColor( VColorRef& color )  
	{
		m_colorInActive.SetRGBA( color.r, color.g, color.b, color.a );
	}
	
protected:
  /// \brief
  ///   Overridden Update function which is called by the virtual thumb stick
  ///   object itself every frame.
  virtual void Update(float fTimeDiff) HKV_OVERRIDE;

private:
  // Member variables
  int m_iRingWidth, m_iRingHeight;
  int m_iCircleWidth, m_iCircleHeight;
  int m_iCircleCenterX, m_iCircleCenterY;
  
  float m_fXValue, m_fYValue;
  float m_fRelativeInitialX, m_fRelativeInitialY;
  
  VisScreenMaskPtr m_spCircleMask;
  VisScreenMaskPtr m_spRingMask;
  
  float m_fTimeDiff;
  
  bool m_bActive;
  
  VTouchAreaPtr m_spTouchArea;
  VRectanglef m_validArea;
  int m_iLastTouchPointIndex;
	const float m_fPriority;												/// The priority value to use when setting up the touch area
	/// Factors for the size of the touch area after a touch has been recognised.
	/// These values scale the size of the touch area, which would normally be the 
	/// same size as m_spRingMask image. value of 1.0 gives the same size area as the ring mask.
	float m_fAfterTouchSizeX, m_fAfterTouchSizeY;		

	VColorRef m_colorActive;					/// The color of the stick images when they are active
	VColorRef m_colorInActive;				/// The color of the stick images when they are not active

	/// thumb stick control mappings, these default to being a left stick
	VInputControl m_CTpadUp,m_CTpadDown,m_CTpadLeft,m_CTpadRight,m_CTpadChanged;
};

}	// end namespace block

#endif

#endif

/*
 * Havok SDK - Base file, BUILD(#20140618)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2014
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
