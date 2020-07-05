#include <stdio.h>

struct vertex {
	float x, y, z, w;
};
struct tris {
	vertex a, b, c;
	char color;
};

class GPU {
public:
	GPU(int w, int h) : w_(w), h_(h) {
		color_buffer_ = new char *[w_];
		for (int i = 0; i < w_; ++i)
			color_buffer_[i] = new char[h_];
		
		depth_buffer_ = new float *[w_];
		for (int i = 0; i < w_; ++i)
			depth_buffer_[i] = new float[h_];

		clearColor();
		clearDepth();
		
		float I[4][4];
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				I[i][j] = i == j;
			
		setModelMatrix(I);
		setViewMatrix(I);
		setProjMatrix(I);
	}
	
	~GPU() {
		for (int i = 0; i < w_; ++i)
			delete[] color_buffer_[i];
		delete[] color_buffer_;

		for (int i = 0; i < w_; ++i)
			delete[] depth_buffer_[i];
		delete[] depth_buffer_;
	}
	
	void clearColor() {
		for (int y = 0; y < h_; ++y) {
			for (int x = 0; x < w_; ++x) {
				color_buffer_[x][y] = ' ';
			}
		}
	}
	
	void clearDepth() {
		for (int y = 0; y < h_; ++y) {
			for (int x = 0; x < w_; ++x) {
				depth_buffer_[x][y] = 1.0f;
			}
		}
	}
	
	void flip() {
		for (int y = 0; y < h_; ++y) {
			for (int x = 0; x < w_; ++x) {
				putchar(color_buffer_[x][y]);
			}
			putchar('\n');
		}
	}
	
	void matrixMultiply(float A[4][4], float B[4][4], float C[4][4]) {
		C[0][0] = A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0] + A[0][3]*B[3][0];
		C[1][0] = A[1][0]*B[0][0] + A[1][1]*B[1][0] + A[1][2]*B[2][0] + A[1][3]*B[3][0];
		C[2][0] = A[2][0]*B[0][0] + A[2][1]*B[1][0] + A[2][2]*B[2][0] + A[2][3]*B[3][0];
		C[3][0] = A[3][0]*B[0][0] + A[3][1]*B[1][0] + A[3][2]*B[2][0] + A[3][3]*B[3][0];
		
		C[0][1] = A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1] + A[0][3]*B[3][1];
		C[1][1] = A[1][0]*B[0][1] + A[1][1]*B[1][1] + A[1][2]*B[2][1] + A[1][3]*B[3][1];
		C[2][1] = A[2][0]*B[0][1] + A[2][1]*B[1][1] + A[2][2]*B[2][1] + A[2][3]*B[3][1];
		C[3][1] = A[3][0]*B[0][1] + A[3][1]*B[1][1] + A[3][2]*B[2][1] + A[3][3]*B[3][1];
		
		C[0][2] = A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2] + A[0][3]*B[3][2];
		C[1][2] = A[1][0]*B[0][2] + A[1][1]*B[1][2] + A[1][2]*B[2][2] + A[1][3]*B[3][2];
		C[2][2] = A[2][0]*B[0][2] + A[2][1]*B[1][2] + A[2][2]*B[2][2] + A[2][3]*B[3][2];
		C[3][2] = A[3][0]*B[0][2] + A[3][1]*B[1][2] + A[3][2]*B[2][2] + A[3][3]*B[3][2];
		
		C[0][3] = A[0][0]*B[0][3] + A[0][1]*B[1][3] + A[0][2]*B[2][3] + A[0][3]*B[3][3];
		C[1][3] = A[1][0]*B[0][3] + A[1][1]*B[1][3] + A[1][2]*B[2][3] + A[1][3]*B[3][3];
		C[2][3] = A[2][0]*B[0][3] + A[2][1]*B[1][3] + A[2][2]*B[2][3] + A[2][3]*B[3][3];
		C[3][3] = A[3][0]*B[0][3] + A[3][1]*B[1][3] + A[3][2]*B[2][3] + A[3][3]*B[3][3];
	}
	
	void renderTris(tris *t, int n) {
		float MV[4][4];
		float MVP[4][4];
		matrixMultiply(model_, view_, MV);
		matrixMultiply(MV, proj_, MVP);
		
		for (int i = 0; i < n; ++i) {
			vertex vtx[3];
			int nearClip = 0;

			vtx[0].w = t[i].a.x * MVP[3][0] + t[i].a.y * MVP[3][1] + t[i].a.z * MVP[3][2] + t[i].a.w * MVP[3][3];
			vtx[0].x = (t[i].a.x * MVP[0][0] + t[i].a.y * MVP[0][1] + t[i].a.z * MVP[0][2] + t[i].a.w * MVP[0][3])/vtx[0].w;
			vtx[0].y = (t[i].a.x * MVP[1][0] + t[i].a.y * MVP[1][1] + t[i].a.z * MVP[1][2] + t[i].a.w * MVP[1][3])/vtx[0].w;
			vtx[0].z = (t[i].a.x * MVP[2][0] + t[i].a.y * MVP[2][1] + t[i].a.z * MVP[2][2] + t[i].a.w * MVP[2][3])/vtx[0].w;
			vtx[0].w = 1.0f;
			if (vtx[0].z < 0.0f) nearClip++;
			
			vtx[1].w = t[i].b.x * MVP[3][0] + t[i].b.y * MVP[3][1] + t[i].b.z * MVP[3][2] + t[i].b.w * MVP[3][3];
			vtx[1].x = (t[i].b.x * MVP[0][0] + t[i].b.y * MVP[0][1] + t[i].b.z * MVP[0][2] + t[i].b.w * MVP[0][3])/vtx[1].w;
			vtx[1].y = (t[i].b.x * MVP[1][0] + t[i].b.y * MVP[1][1] + t[i].b.z * MVP[1][2] + t[i].b.w * MVP[1][3])/vtx[1].w;
			vtx[1].z = (t[i].b.x * MVP[2][0] + t[i].b.y * MVP[2][1] + t[i].b.z * MVP[2][2] + t[i].b.w * MVP[2][3])/vtx[1].w;
			vtx[1].w = 1.0f;
			if (vtx[1].z < 0.0f) nearClip++;
			
			vtx[2].w = t[i].c.x * MVP[3][0] + t[i].c.y * MVP[3][1] + t[i].c.z * MVP[3][2] + t[i].c.w * MVP[3][3];
			vtx[2].x = (t[i].c.x * MVP[0][0] + t[i].c.y * MVP[0][1] + t[i].c.z * MVP[0][2] + t[i].c.w * MVP[0][3])/vtx[2].w;
			vtx[2].y = (t[i].c.x * MVP[1][0] + t[i].c.y * MVP[1][1] + t[i].c.z * MVP[1][2] + t[i].c.w * MVP[1][3])/vtx[2].w;
			vtx[2].z = (t[i].c.x * MVP[2][0] + t[i].c.y * MVP[2][1] + t[i].c.z * MVP[2][2] + t[i].c.w * MVP[2][3])/vtx[2].w;
			vtx[2].w = 1.0f;
			if (vtx[2].z < 0.0f) nearClip++;
			
			if (nearClip == 3)
				continue;
			
			// TODO: nearClipping
			
			int x_min = (vtx[0].x < vtx[1].x ? (vtx[0].x < vtx[2].x ? vtx[0].x : vtx[2].x) : (vtx[1].x < vtx[2].x ? vtx[1].x : vtx[2].x))*w_;
			int x_max = (vtx[0].x > vtx[1].x ? (vtx[0].x > vtx[2].x ? vtx[0].x : vtx[2].x) : (vtx[1].x > vtx[2].x ? vtx[1].x : vtx[2].x))*w_;
			int y_min = (vtx[0].y < vtx[1].y ? (vtx[0].y < vtx[2].y ? vtx[0].y : vtx[2].y) : (vtx[1].y < vtx[2].y ? vtx[1].y : vtx[2].y))*h_;
			int y_max = (vtx[0].y > vtx[1].y ? (vtx[0].y > vtx[2].y ? vtx[0].y : vtx[2].y) : (vtx[1].y > vtx[2].y ? vtx[1].y : vtx[2].y))*h_;
			if (x_min < 0) x_min = 0;
			if (x_max > w_) x_max = w_;
			if (y_min < 0) y_min = 0;
			if (y_max > h_) y_max = h_;
		
			float det = 1.0f/((vtx[1].y-vtx[2].y)*(vtx[0].x-vtx[2].x) + (vtx[2].x-vtx[1].x)*(vtx[0].y-vtx[2].y));
			for (int y = y_min; y < y_max; ++y) {
				for (int x = x_min; x < x_max; ++x) {
					float l0 = ((vtx[1].y-vtx[2].y)*((float)x/w_-vtx[2].x)+(vtx[2].x-vtx[1].x)*((float)y/h_-vtx[2].y))*det;
					float l1 = ((vtx[2].y-vtx[0].y)*((float)x/w_-vtx[2].x)+(vtx[0].x-vtx[2].x)*((float)y/h_-vtx[2].y))*det;
					float l2 = 1.0f - l0 - l1;
					if (l0 >= 0 && l1 >= 0 && l2 >= 0) {
						float depth = vtx[0].z*l0 + vtx[1].z*l1 + vtx[2].z*l2; 
						if (depth <= depth_buffer_[x][y]) {
							depth_buffer_[x][y] = depth;
							color_buffer_[x][y] = t[i].color;
						}
					}
				}
			}
		}
	}
	
	void setModelMatrix(float M[4][4]) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				model_[i][j] = M[i][j];
	}
	
	void setViewMatrix(float M[4][4]) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				view_[i][j] = M[i][j];
	}
	
	void setProjMatrix(float M[4][4]) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				proj_[i][j] = M[i][j];
	}

private:
	int w_, h_;
	char **color_buffer_ = nullptr;
	float **depth_buffer_ = nullptr;
	float model_[4][4];
	float view_[4][4];
	float proj_[4][4];
};


int main (int argc, char *argv[]) {
	GPU gpu = GPU(80, 24);
	tris T[2];
	
	T[0].a.x = 0.0;
	T[0].a.y = 0.0;
	T[0].a.z = 1.0;
	T[0].a.w = 1.0;
	
	T[0].b.x = 1.0;
	T[0].b.y = 0.0;
	T[0].b.z = 1.0;
	T[0].b.w = 1.0;
	
	T[0].c.x = 0.5;
	T[0].c.y = 1.0;
	T[0].c.z = 1.0;
	T[0].c.w = 1.0;
	
	T[0].color = 'A';
	
	T[1].a.x = 0.5;
	T[1].a.y = 0.0;
	T[1].a.z = 0.1;
	T[1].a.w = 1.0;
	
	T[1].b.x = 0.3;
	T[1].b.y = 1.0;
	T[1].b.z = 0.1;
	T[1].b.w = 1.0;
	
	T[1].c.x = 0.7;
	T[1].c.y = 1.0;
	T[1].c.z = 0.1;
	T[1].c.w = 1.0;
	
	T[1].color = 'B';
	
	gpu.renderTris(T, 2);
	gpu.flip();
	return 0;
}
