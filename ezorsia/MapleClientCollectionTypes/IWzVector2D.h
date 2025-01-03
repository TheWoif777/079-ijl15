#pragma once
#undef  INTERFACE
#define INTERFACE   IWzVector2D

DECLARE_INTERFACE_IID_(IWzVector2D, IUnknown, "F28BD1ED-3DEB-4F92-9EEC-10EF5A1C3FB4")
{
	BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzShape2D methods ***/
    STDMETHOD(get_item)(THIS_ tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(get__NewEnum)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_count)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_x)(THIS_ int*) PURE;
    STDMETHOD(put_x)(THIS_ int) PURE;
    STDMETHOD(get_y)(THIS_ int*) PURE;
    STDMETHOD(put_y)(THIS_ int) PURE;
    STDMETHOD(get_x2)(THIS_ int*) PURE;
    STDMETHOD(put_x2)(THIS_ int) PURE;
    STDMETHOD(get_y2)(THIS_ int*) PURE;
    STDMETHOD(put_y2)(THIS_ int) PURE;
    STDMETHOD(raw_Move)(THIS_ int nX, int nY) PURE;
    STDMETHOD(raw_Offset)(THIS_ int nDX, int nDY) PURE;
    STDMETHOD(raw_Scale)(THIS_ int nXMul, int nXDiv, int nYMul, int nYDiv, int nXOrg, int nYOrg) PURE;
    STDMETHOD(raw_Insert)(THIS_ tagVARIANT vIndexOrShape, tagVARIANT vShape) PURE;
    STDMETHOD(raw_Remove)(THIS_ tagVARIANT vIndex, tagVARIANT * pvRemoved) PURE;
    STDMETHOD(raw_Init)(THIS_ int nX, int nY) PURE;

    /*** IWzVector2D methods ***/
    STDMETHOD(get_currentTime)(THIS_ int* pnTime) PURE;
    STDMETHOD(put_currentTime)(THIS_ int nTime) PURE;
    STDMETHOD(get_origin)(THIS_ tagVARIANT * ppOrigin) PURE;
    STDMETHOD(put_origin)(THIS_ tagVARIANT pOrigin) PURE;
    STDMETHOD(get_rx)(THIS_ int* pnX) PURE;
    STDMETHOD(put_rx)(THIS_ int nX) PURE;
    STDMETHOD(get_ry)(THIS_ int* pnY) PURE;
    STDMETHOD(put_ry)(THIS_ int nY) PURE;
    STDMETHOD(get_a)(THIS_ long double* pa) PURE;
    STDMETHOD(get_ra)(THIS_ long double* pra) PURE;
    STDMETHOD(put_ra)(THIS_ long double ra) PURE;
    STDMETHOD(get_flipX)(THIS_ int* pnFlipX) PURE;
    STDMETHOD(put_flipX)(THIS_ int nFlipX) PURE;
    STDMETHOD(raw__GetSnapshot)(THIS_ int* px, int* py, int* prx, int* pry, int* pxOrg, int* pyOrg, long double* pa, long double* paOrg, tagVARIANT vTime) PURE;
    STDMETHOD(raw_RelMove)(THIS_ int nX, int nY, _variant_t nTime, _variant_t nType) PURE;
    STDMETHOD(raw_RelOffset)(THIS_ int nDX, int nDY, tagVARIANT nTime, tagVARIANT nType) PURE;
    STDMETHOD(raw_Ratio)(THIS_ IWzVector2D * pOrigin, int nOriginWidth, int nOriginHeight, int nScaleWidth, int nScaleHeight) PURE;
    STDMETHOD(raw_WrapClip)(THIS_ tagVARIANT pOrigin, int nWrapLeft, int nWrapTop, unsigned int uWrapWidth, unsigned int uWrapHeight, tagVARIANT bClip) PURE;
    STDMETHOD(raw_Rotate)(THIS_ long double aAmount, tagVARIANT nTime) PURE;
    STDMETHOD(get_looseLevel)(THIS_ unsigned int* puLevel) PURE;
    STDMETHOD(put_looseLevel)(THIS_ unsigned int uLevel) PURE;
    STDMETHOD(raw_Fly)(THIS_ tagVARIANT * aVar, int nArgs) PURE;

	int get_x()//
	{
		int x;
		this->get_x(&x);

		return x;
	}

	int get_y()
	{
		int y;
		this->get_y(&y);

		return y;
	}

	END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzVector2D, __uuidof(IWzVector2D));