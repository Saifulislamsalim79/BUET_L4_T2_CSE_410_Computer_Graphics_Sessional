#include <bits/stdc++.h>
#define RAD(x) (((x)*PI)/180.0)
using namespace std;

const double PI=acos(-1.0);

class Matrix {
public:
    int dim[2];
    vector< vector<double> >mat;

    Matrix() {}

    Matrix(int d0,int d1)
    {
        vector<double>temp;
        int i;
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) temp.push_back(0);
        for(i=0;i<d0;i++) mat.push_back(temp);
    }


    void SetDim(int d0,int d1)
    {
        vector<double>temp;
        int i;
        mat.clear();
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) temp.push_back(0);
        for(i=0;i<d0;i++) mat.push_back(temp);
    }


    Matrix (const Matrix &m)
    {
        this->SetDim(m.dim[0],m.dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                mat[i][j]=m.mat[i][j];
    }

    Matrix operator * (const Matrix &m)
    {
    	assert(dim[1]==m.dim[0]);
    	Matrix ret=Matrix(dim[0],m.dim[1]);
    	int i,j,k;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<m.dim[1];j++)
                for(k=0;k<dim[1];k++)
                    ret.mat[i][j]+=mat[i][k]*m.mat[k][j];
	    return ret;
    }


    Matrix operator * (double c)
    {
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.mat[i][j]=mat[i][j]*c;
	    return ret;
    }


    Matrix operator + (const Matrix &m)
    {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.mat[i][j]=mat[i][j]+m.mat[i][j];
	    return ret;
    }


    Matrix operator - (const Matrix &m)
    {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ret.mat[i][j]=mat[i][j]-m.mat[i][j];
	    return ret;
    }


    void Clear()
    {
        int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                mat[i][j]=0;
    }
};



class HomogeneousPoint : public Matrix {
public:
    HomogeneousPoint()
    {
        this->SetDim(4,1);
    }


    void operator = (const Matrix &m)
    {
        assert(m.dim[0]==4&&m.dim[1]==1);
    	int i,j;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                mat[i][j]=m.mat[i][j];
    }


    double dot (const  HomogeneousPoint &p)
    {
    	assert(dim[0]==p.dim[0]);
    	int i,j;
    	double ans=0;
	    for(i=0;i<dim[0];i++)
            for(j=0;j<dim[1];j++)
                ans+=mat[i][j]*p.mat[i][j];
	    return ans;
    }


    void Normalize()
    {
        int i;
        for(i=0;i<3;i++) mat[i][0]/=mat[3][0];
    }


    void MakeUnit()
    {
        int i;
        double m=0;
        for(i=0;i<3;i++) m+=mat[i][0]*mat[i][0];
        m=sqrt(m);
        for(i=0;i<3;i++) mat[i][0]/=m;
        mat[3][0]=1;
    }


    void Read()
    {
        int i;
        for(i=0;i<3;i++) cin>>mat[i][0];
        mat[3][0]=1;
    }


    void Show(FILE *outputFile)
    {
        int i;
        for(i=0;i<3;i++) fprintf(outputFile,"%.7lf ",mat[i][0]);
        fprintf(outputFile,"\n");
    }
};



 HomogeneousPoint Cross( HomogeneousPoint mat, HomogeneousPoint b)
 {
    HomogeneousPoint Res;
    Res.mat[0][0]=mat.mat[1][0]*b.mat[2][0]-mat.mat[2][0]*b.mat[1][0];
    Res.mat[1][0]=mat.mat[2][0]*b.mat[0][0]-mat.mat[0][0]*b.mat[2][0];
    Res.mat[2][0]=mat.mat[0][0]*b.mat[1][0]-mat.mat[1][0]*b.mat[0][0];
    Res.mat[3][0]=1;
    return Res;
}


 HomogeneousPoint Rotate( HomogeneousPoint X, HomogeneousPoint A,double angle)
 {
    HomogeneousPoint W,Xp;
    A.MakeUnit();
    W=Cross(A,X);
    Xp=X-A*(A.dot(X)-1);
    X=A*(A.dot(X)-1)+Xp*cos(RAD(angle))+W*sin(RAD(angle));
    return X;
}


Matrix GetRotationMatrix( HomogeneousPoint A,double angle)
{
    Matrix R(4,4);
    HomogeneousPoint W,X;
    int i;

    X.mat[0][0]=1;
    X.mat[1][0]=0;
    X.mat[2][0]=0;
    X.mat[3][0]=1;
    X=Rotate(X,A,angle);
    for(i=0;i<3;i++) R.mat[i][0]=X.mat[i][0];

    X.mat[0][0]=0;
    X.mat[1][0]=1;
    X.mat[2][0]=0;
    X.mat[3][0]=1;
    X=Rotate(X,A,angle);
    for(i=0;i<3;i++) R.mat[i][1]=X.mat[i][0];

    X.mat[0][0]=0;
    X.mat[1][0]=0;
    X.mat[2][0]=1;
    X.mat[3][0]=1;
    X=Rotate(X,A,angle);
    for(i=0;i<3;i++) R.mat[i][2]=X.mat[i][0];

    for(i=0;i<3;i++) R.mat[3][i]=R.mat[i][3]=0;
    R.mat[3][3]=1;
    return R;
}


void ApplyTransform( HomogeneousPoint &p,vector<Matrix>&tr,int exact=-1)
{
    int i;
    if(exact==-1) {
        for(i=tr.size()-1;i>=2;i--)
        {
            p=tr[i]*p;
            p.Normalize();
        }
    }
    else
    {
        p=tr[exact]*p;
        p.Normalize();
    }
}

vector<Matrix>matrixStack;

int main() {
    int i;
    double x,y,z,ang,fovY,fovX,asp,yLim,xLim,near,far;
    HomogeneousPoint p,eye,lookAt,r,l,u;
    Matrix I(4,4),R(4,4);

    string command;
    FILE *stage[3];

    freopen("scene.txt","r",stdin);
    stage[0]=fopen("stage1.txt","w");
    stage[1]=fopen("stage2.txt","w");
    stage[2]=fopen("stage3.txt","w");

    for(i=0;i<4;i++) I.mat[i][i]=1;
    matrixStack.push_back(I);

    eye.Read();
    lookAt.Read();
    u.Read();

    l=lookAt-eye;
    l.MakeUnit();
    r=Cross(l,u);
    r.MakeUnit();
    u=Cross(r,l);

    R.Clear();
    R.mat[0][0]=r.mat[0][0];
    R.mat[0][1]=r.mat[1][0];
    R.mat[0][2]=r.mat[2][0];

    R.mat[1][0]=u.mat[0][0];
    R.mat[1][1]=u.mat[1][0];
    R.mat[1][2]=u.mat[2][0];

    R.mat[2][0]=-l.mat[0][0];
    R.mat[2][1]=-l.mat[1][0];
    R.mat[2][2]=-l.mat[2][0];

    R.mat[3][3]=1;
    matrixStack.push_back(R);



    R.Clear();
    R.mat[0][0]=1;
    R.mat[1][1]=1;
    R.mat[2][2]=1;

    R.mat[0][3]=-eye.mat[0][0];
    R.mat[1][3]=-eye.mat[1][0];
    R.mat[2][3]=-eye.mat[2][0];

    R.mat[3][3]=1;
    matrixStack[1]=matrixStack[1]*R;



    cin>>fovY>>asp>>near>>far;
    fovX=fovY*asp;
    yLim=near*tan(RAD(fovY/2));
    xLim=near*tan(RAD(fovX/2));


    R.Clear();
    R.mat[0][0]=near/xLim;
    R.mat[1][1]=near/yLim;
    R.mat[2][2]=-(far+near)/(far-near);
    R.mat[2][3]=-(2*far*near)/(far-near);
    R.mat[3][2]=-1;
    matrixStack[0]=R;

    matrixStack.push_back(I);

    while(true) {
        cin>>command;

        if(command=="triangle")
        {
            for(i=0;i<3;i++)
            {
                p.Read();
                ApplyTransform(p,matrixStack);
                p.Show(stage[0]);
                ApplyTransform(p,matrixStack,1);
                p.Show(stage[1]);
                ApplyTransform(p,matrixStack,0);
                p.Show(stage[2]);
            }
            for(i=0;i<3;i++) fprintf(stage[i],"\n");
        }

        else if(command=="scale")
        {
            cin>>x>>y>>z;
            R.Clear();
            R.mat[0][0]=x;
            R.mat[1][1]=y;
            R.mat[2][2]=z;
            R.mat[3][3]=1;
            matrixStack[matrixStack.size()-1]=matrixStack[matrixStack.size()-1]*R;
        }

        else if(command=="translate")
        {
            cin>>x>>y>>z;
            R.Clear();
            R.mat[0][0]=1;
            R.mat[1][1]=1;
            R.mat[2][2]=1;

            R.mat[0][3]=x;
            R.mat[1][3]=y;
            R.mat[2][3]=z;

            R.mat[3][3]=1;
            matrixStack[matrixStack.size()-1]=matrixStack[matrixStack.size()-1]*R;
        }

        else if(command=="rotate")
        {
            cin>>ang;
            p.Read();
            R=GetRotationMatrix(p,ang);
            matrixStack[matrixStack.size()-1]=matrixStack[matrixStack.size()-1]*R;
        }

        else if(command=="push")
        {
            matrixStack.push_back(I);
        }

        else if(command=="pop")
        {
            matrixStack.pop_back();
        }

        else if(command=="end")
        {
            break;
        }
    }
    return 0;
}
