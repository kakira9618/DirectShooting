//�w�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <vector>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
//�x����\��
#pragma warning(disable : 4305)
#include "Constant.h"
float M_PI = acos(-1);
//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
	D3DXVECTOR4 Color; //�ʒu
	D3DXVECTOR4 dx; // ���_�̃Y��
};
//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
};
//���̂̍\����
struct MODEL//���̃T���v���ł̓v����
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 dx[PUDDING_REF_NUMV];
	D3DXVECTOR3 v[PUDDING_REF_NUMV];
};
//�e�̍\����
struct BULLET
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 v;
	bool alive;
	int time;
};

//
//
//
class MAIN
{
public:
	MAIN();
	~MAIN();
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();

	void Loop();
	void App();
	void Update();
	void Render();
	void DestroyD3D();
	void UpdatePuddingForm();

	std::vector<SimpleVertex> makePuddingVertices();
	std::vector<WORD> makePuddingIndices();
	std::vector<SimpleVertex> makeBulletVertices();
	std::vector<WORD> makeBulletIndices();

	//���A�v���ɂЂƂ�
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������	
	ID3D11Buffer* m_pBulletVertexBuffer;
	ID3D11Buffer* m_pBulletIndexBuffer;
	ID3D11Buffer* m_pPuddingVertexBuffer;
	ID3D11Buffer* m_pPuddingIndexBuffer;
	std::vector<SimpleVertex> m_puddingVertices;
	std::vector<WORD> m_puddingIndices;
	std::vector<SimpleVertex> m_bulletVertices;
	std::vector<WORD> m_bulletIndices;
	//���f���̃C���X�^���X�z��
	MODEL m_Model[MAX_MODEL];
	BULLET m_BulletModel[MAX_BULLET_NUM];
	int m_iNumModel;

	//�J�����֌W
	D3DXVECTOR3 m_campos;
	D3DXVECTOR3 m_camlook;
	float m_cam_vtheta;
};
