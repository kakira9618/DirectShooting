#include "Constant.h"
//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_PolyColor;//ポリゴン色
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION0 , float4 Color : COLOR0, float4 dx : POSITION_DX)
{   
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 d = float4(dx.x, dx.y, dx.z, 0);
	output.Pos=mul(Pos + d, g_mWVP);
	output.Color = Color;

    return output;  
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}