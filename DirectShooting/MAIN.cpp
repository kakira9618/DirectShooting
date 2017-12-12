#include "MAIN.h"
//�O���[�o���ϐ�
MAIN* g_pMain=NULL;
//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//�A�v���P�[�V�����̃G���g���[�֐� 
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if(SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}
		//�A�v���I��
		g_pMain->DestroyD3D();
		delete g_pMain;
	}
	return 0;
}
//
//
//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//
MAIN::MAIN()
{
	ZeroMemory(this,sizeof(MAIN));
}
//
//
//
MAIN::~MAIN()
{
}
//
//
//�E�B���h�E�쐬
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPCWSTR WindowName)
 {
	 // �E�B���h�E�̒�`
	 WNDCLASSEX  wc;
	 ZeroMemory(&wc,sizeof(wc));
	 wc.cbSize = sizeof(wc);
	 wc.style = CS_HREDRAW | CS_VREDRAW;
	 wc.lpfnWndProc = WndProc;
	 wc.hInstance = hInstance;
	 wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	 wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	 wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	 wc.lpszClassName = WindowName;
	 wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	 RegisterClassEx(&wc);
	 //�E�B���h�E�̍쐬
	 m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
		 0,0,iWidth,iHeight,0,0,hInstance,0);
	 if(!m_hWnd)
	 {
		 return E_FAIL;
	 }
	 //�E�C���h�E�̕\��
	 ShowWindow(m_hWnd,SW_SHOW);
	 UpdateWindow(m_hWnd) ;
	 
	 return S_OK;
 }
//
//
//�E�B���h�E�v���V�[�W���[
 LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESC�L�[�ŏC��
			PostQuitMessage(0);
			break;
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
 }
//
//
//���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����
 void MAIN::Loop()
 {
	 // ���b�Z�[�W���[�v
	 MSG msg={0};
	 ZeroMemory(&msg,sizeof(msg));
	 while(msg.message!=WM_QUIT)
	 {
		 if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 else
		 {
			 //�A�v���P�[�V�����̏����͂��������ԁB
			 App();
		 }
	 }
	 //�A�v���P�[�V�����̏I��
}
//
//
//�A�v���P�[�V���������B�A�v���̃��C���֐��B
void MAIN::App() 
{
	Update();
	Render();
}

void MAIN::Update() 
{
	// �J�����ړ�
	float cam_speed = 0.3;
	if (GetKeyState('W') & 0x8000) {
		m_campos.y+= cam_speed;
	}
	if (GetKeyState('A') & 0x8000) {
		m_campos.x-= cam_speed;
	}
	if (GetKeyState('S') & 0x8000) {
		m_campos.y -= cam_speed;
	}
	if (GetKeyState('D') & 0x8000) {
		m_campos.x += cam_speed;
	}
	if (GetKeyState('Z') & 0x8000) {
		m_campos.z -= cam_speed;
	}
	if (GetKeyState('X') & 0x8000) {
		m_campos.z += cam_speed;
	}
	if (GetKeyState('Q') & 0x8000) {
		m_cam_vtheta += 0.01f;
		m_camlook.y = 2.0 * sin(m_cam_vtheta);
		m_camlook.z = 2.0 * cos(m_cam_vtheta);
	}
	if (GetKeyState('E') & 0x8000) {
		m_cam_vtheta -= 0.01f;
		m_camlook.y = 2.0 * sin(m_cam_vtheta);
		m_camlook.z = 2.0 * cos(m_cam_vtheta);
	}

	// �e����
	static int sp_frame = 0;
	if (GetKeyState(VK_SPACE) & 0x8000) {
		sp_frame++;
	}
	else {
		sp_frame = 0;
	}
	if (sp_frame % 10 == 1) {
		for (int i = 0; i < MAX_BULLET_NUM; i++) {
			if (!m_BulletModel[i].alive) {
				m_BulletModel[i].alive = true;
				m_BulletModel[i].vPos = m_campos;
				m_BulletModel[i].time = 0;
				m_BulletModel[i].v = 1 * D3DXVECTOR3(0, sin(m_cam_vtheta), cos(m_cam_vtheta));
				break;
			}
		}
	}

	// �I�u�W�F�N�g�ړ�
	for (int i = 0; i<m_iNumModel; i++)
	{
		m_Model[i].vPos.z -= 0.01f;
	}
	for (int i = 0; i<MAX_BULLET_NUM; i++)
	{
		if (m_BulletModel[i].alive) {
			m_BulletModel[i].vPos += m_BulletModel[i].v;
			m_BulletModel[i].time++;
			if (m_BulletModel[i].time > 300) {
				m_BulletModel[i].alive = false;
			}
		}
	}

	UpdatePuddingForm();

}

void MAIN::UpdatePuddingForm() {
	// GPGPU���g���č����������������l��������
	float pudding_strength = 0.99;
	float force_strength = 0.2;
	float k_spring = 0.50;
	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		BULLET &bullet = m_BulletModel[i];
		D3DXVECTOR3 &bPos = bullet.vPos;
		for (int j = 0; j < MAX_MODEL; j++) {
			MODEL &pudding = m_Model[j];
			for (int k = 0; k < PUDDING_REF_NUMV; k++) {
				D3DXVECTOR3 &dx = D3DXVECTOR3(pudding.dx[k].x, pudding.dx[k].y, pudding.dx[k].z);
				D3DXVECTOR3 &tPos = m_puddingVertices[k].Pos;
				D3DXVECTOR3 &pos = dx + tPos + pudding.vPos;
				D3DXVECTOR3 &v = pudding.v[k];
				D3DXVECTOR3 &b_d = pos - bPos;

				dx += v;
				pudding.dx[k] = D3DXVECTOR4(dx, 0.0f);

				v *= pudding_strength;
				float dist = sqrt(b_d.x * b_d.x + b_d.y * b_d.y + b_d.z * b_d.z);
				if (dist < 0.01) { dist = 0.01; }
				if (bullet.alive) {
					v += force_strength * (1.0 / dist) * b_d;
				}
				v -= k_spring * dx;
			}
		}
	}
}

//
//
//
HRESULT MAIN::InitD3D()
{
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width=WINDOW_WIDTH;
	sd.BufferDesc.Height=WINDOW_HEIGHT;
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator=60;
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow=m_hWnd;
	sd.SampleDesc.Count=1;
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;
	
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )) )
	{
		return FALSE;
	}
	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer);    
	m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
	SAFE_RELEASE(pBackBuffer);	
	//�[�x�X�e���V���r���[�̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	m_pDevice->CreateDepthStencilView( m_pDepthStencil, NULL, &m_pDepthStencilView );
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView,m_pDepthStencilView);
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &vp );
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr=NULL;
	m_pDevice->CreateRasterizerState(&rdc,&pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
	//�V�F�[�_�[������
	if(FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	if(FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pPuddingVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//
//�V�F�[�_�[���쐬�@���_���C�A�E�g���`
HRESULT MAIN::InitShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors=NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if(FAILED(D3DX11CompileFromFile(L"Simple.hlsl",NULL,NULL,"VS","vs_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,L"hlsl�ǂݍ��ݎ��s",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s",NULL,MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION_DX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if( FAILED( m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if(FAILED(D3DX11CompileFromFile(L"Simple.hlsl",NULL,NULL,"PS","ps_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,L"hlsl�ǂݍ��ݎ��s",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"�s�N�Z���V�F�[�_�[�쐬���s",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);		
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SIMPLESHADER_CONSTANT_BUFFER );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	=0;
	cb.StructureByteStride=0;
	cb.Usage=D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}

std::vector<WORD> MAIN::makeBulletIndices() {
	int divT = BULLET_DIVT;
	int divH = BULLET_DIVH;

	std::vector<WORD> ret;

	for (DWORD hh = 0; hh < divH; hh++) {
		for (DWORD tt = 0; tt < divT; tt++) {
			WORD u0 = divT * hh + tt;
			WORD u1 = divT * (hh + 1) + tt;
			WORD v0 = divT * hh + (tt + 1) % divT;
			WORD v1 = divT * (hh + 1) + (tt + 1) % divT;
			ret.push_back(u0);
			ret.push_back(u1);
			ret.push_back(v1);
			ret.push_back(u0);
			ret.push_back(v1);
			ret.push_back(v0);
		}
	}

	return ret;
}


std::vector<SimpleVertex> MAIN::makeBulletVertices() {
	int divT = BULLET_DIVT;
	int divH = BULLET_DIVH;
	float r = 0.5f;
	std::vector<SimpleVertex> ret;
	D3DXVECTOR4 color(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	D3DXVECTOR4 dx(0.0f, 0.0f, 0.0f, 0.0f);
	for (int hh = 0; hh < divH + 1; hh++) {
		float y = 2 * r * ((1.0f * hh / divH) - 0.5);
		float rr = sqrt(r * r - y * y);
		for (int tt = 0; tt < divH; tt++) {
			float theta = ((360.0 / divT) * tt) / 180.0 * M_PI;
			D3DXVECTOR3 v(rr * cos(theta), y, rr * sin(theta));
			ret.push_back({ v, color, dx });
		}
	}
	return ret;
}


std::vector<WORD> MAIN::makePuddingIndices() {
	int divR = PUDDING_DIVR;
	int divT = PUDDING_DIVT;
	std::vector<WORD> ret;
	
	for (DWORD rr = 0; rr < divR; rr++) {
		for (DWORD tt = 0; tt < divT; tt++) {
			WORD u0 = divT * rr + tt;
			WORD u1 = divT * (rr + 1) + tt;
			WORD v0 = divT * rr + (tt + 1) % divT;
			WORD v1 = divT * (rr + 1) + (tt + 1) % divT;
			ret.push_back(u0);
			ret.push_back(u1);
			ret.push_back(v1);
			ret.push_back(u0);
			ret.push_back(v1);
			ret.push_back(v0);
		}
	}

	return ret;
}

std::vector<SimpleVertex> MAIN::makePuddingVertices() {
	int divR = PUDDING_DIVR;
	int divT = PUDDING_DIVT;
	std::vector<SimpleVertex> ret;
	D3DXVECTOR4 dx(0.0f, 0.0f, 0.0f, 0.0f);
	for (int rr = 0; rr < (divR + 1); rr++) {
		float r = rr * 0.1;
		D3DXVECTOR4 color(90.0f / 255.0f, 15.0f / 255.0f, 1.0f / 255.0f, 1.0f);
		float y = 0.25;
		if (rr >= divR / 3) {
			color = D3DXVECTOR4(255.0f / 255.0f, 255.0f / 255.0f, 181.0f / 255.0f, 1.0f);
			y -= (rr - divR / 3) * 0.05;
			r -= (rr - divR / 3) * 0.07;
		}
		for (int tt = 0; tt < divT; tt++) {
			float theta = ((360.0 / divT) * tt) / 180.0 * M_PI;
			D3DXVECTOR3 v(r * cos(theta), y, r * sin(theta));
			ret.push_back({v, color, dx });
		}
	}
	return ret;
}

HRESULT MAIN::InitPolygon()
{
	// �v�����p�̃o�b�t�@���쐬
	m_puddingVertices = makePuddingVertices();
	m_puddingIndices = makePuddingIndices();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SimpleVertex) * m_puddingVertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_puddingVertices[0];
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_pPuddingVertexBuffer ) ) )
	{
		return E_FAIL;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_puddingIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &m_puddingIndices[0];
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pPuddingIndexBuffer)))
	{
		return E_FAIL;
	}

	// �e�p
	m_bulletVertices = makeBulletVertices();
	m_bulletIndices = makeBulletIndices();

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * m_bulletVertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	InitData.pSysMem = &m_bulletVertices[0];
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pBulletVertexBuffer)))
	{
		return E_FAIL;
	}

	bd.ByteWidth = sizeof(WORD) * m_bulletIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	InitData.pSysMem = &m_bulletIndices[0];
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pBulletIndexBuffer)))
	{
		return E_FAIL;
	}

	//�S�Ẵ��f���œ����|���S���B�����o�[�e�b�N�X�o�b�t�@�[���g���B���f�����ƂɈقȂ�̂́A���f���̈ʒu�ƐF�B	
	for(int i=0;i<MAX_MODEL;i++)
	{
		m_Model[i].vPos=D3DXVECTOR3(float(rand())/1000.0f-16.0f,float(rand())/1000.0f-16.0f,float(rand())/1000.0f+10.0f);//�����ʒu�̓����_��
		m_Model[i].vColor=D3DXVECTOR4(float(rand())/32767.0f,float(rand())/32767.0f,float(rand())/32767.0f,1.0f);//�F�������_��
	}
	m_iNumModel=MAX_MODEL;

	m_campos.z = -2.0f;
	m_camlook.z = 2.0f;
	m_cam_vtheta = 0.0f;

	return S_OK;
}
//
//
//�V�[������ʂɃ����_�����O
void MAIN::Render()
{
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = {0,0,1,1};// �N���A�F�쐬�@RGBA�̏�
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,ClearColor);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);//�[�x�o�b�t�@�N���A

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vEyePt(m_campos.x, m_campos.y, m_campos.z); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt( m_camlook.x + m_campos.x, m_camlook.y + m_campos.y, m_camlook.z + m_campos.z);//�����ʒu
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//����ʒu
	D3DXMatrixLookAtLH( &mView, &vEyePt, &vLookatPt, &vUpVec );
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DXMatrixPerspectiveFovLH( &mProj, D3DX_PI/4, (FLOAT)WINDOW_WIDTH/(FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f );	
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);	
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer );	
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pPuddingVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pPuddingIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//�v���~�e�B�u�������_�����O �@�����Ȃ̂ŁA���[���h�g�����X�t�H�[���Ƃ����n�����������[�v���ɂ����i���f�����Ƃɍs���j
	for(int i=0;i<m_iNumModel;i++)
	{
		//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		D3DXMATRIX mTrans;
		D3DXMatrixTranslation(&mTrans,m_Model[i].vPos.x,m_Model[i].vPos.y,m_Model[i].vPos.z);
		mWorld=mTrans;
		
		std::vector<SimpleVertex> pudding = m_puddingVertices;

		for (int j = 0; j < PUDDING_REF_NUMV; j++) {
			pudding[j].dx = m_Model[i].dx[j];
		}

		// �X�̒��_�̕ψʂ��X�V
		D3D11_MAPPED_SUBRESOURCE msr;
		m_pDeviceContext->Map(m_pPuddingVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, &pudding[0], sizeof(SimpleVertex)*PUDDING_REF_NUMV);
		m_pDeviceContext->Unmap(m_pPuddingVertexBuffer, 0);
		
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		memset(&cb, 0, sizeof(cb));
		if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
		{
			//���[���h�A�J�����A�ˉe�s���n��
			D3DXMATRIX m=mWorld*mView*mProj;
			D3DXMatrixTranspose( &m, &m );
			cb.mWVP=m;
			//�J���[��n��
			cb.vColor=m_Model[i].vColor;
			memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb), sizeof( cb ) );
			m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
		}
		m_pDeviceContext->DrawIndexed(m_puddingIndices.size(), 0, 0);
	}

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pBulletIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		if (!m_BulletModel[i].alive) continue;
		//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		D3DXMATRIX mTrans;
		D3DXMatrixTranslation(&mTrans, m_BulletModel[i].vPos.x, m_BulletModel[i].vPos.y, m_BulletModel[i].vPos.z);
		mWorld = mTrans;

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		memset(&cb, 0, sizeof(cb));
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//���[���h�A�J�����A�ˉe�s���n��
			D3DXMATRIX m = mWorld*mView*mProj;
			D3DXMatrixTranspose(&m, &m);
			cb.mWVP = m;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->DrawIndexed(m_bulletIndices.size(), 0, 0);
	}

	m_pSwapChain->Present(1,0);//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj	
}