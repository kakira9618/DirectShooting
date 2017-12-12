#include "Constant.h"
//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_PolyColor;//�|���S���F
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
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
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}