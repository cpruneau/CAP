#include <TObject.h>
#include <iostream>
#include <fstream>
#include "ChemicalPotential.hpp"
#include "Arsenal.hpp"

ClassImp(ChemicalPotential);

using namespace std;

ChemicalPotential::ChemicalPotential()
{
    read_in_flag = 0;
}

ChemicalPotential::~ChemicalPotential()
{
    delete [] T;
    for(int i = 0; i < nStableParticles; i++) delete [] mu_table[i];
    delete [] mu_table;
    if(read_in_flag == 99) delete EOS_Mu_Table_ptr;
}

void ChemicalPotential::readStableParticleListFromFile(string filename)
{
    read_in_flag = 1;
    ifstream in_file(filename.c_str());
    in_file >> nStableParticles;
    stable_Particle_monval_list = new int [nStableParticles];
    int dummy;
    string temp;
    for(int i = 0; i < nStableParticles; i++)
    {
        in_file >> dummy >> stable_Particle_monval_list[i];
        getline(in_file, temp);
    }
    in_file.close();

    T_chem = 0.165;
    double T_min = 0.05;
    nT = 200;
    double dT = (T_chem - T_min)/(nT - 1);
    T = new double [nT];
    for(int i = 0; i < nT; i++)
        T[i] = T_chem - i*dT;

    mu_table = new double* [nStableParticles];
    for(int i = 0; i < nStableParticles; i++)
    {
        mu_table[i] = new double [nT];
        for(int j = 0; j < nT; j++) mu_table[i][j] = 0.0;
    }
}

void ChemicalPotential::readChemicalPotentialTableFromFile(string filename)
{
    read_in_flag = 99;
    EOS_Mu_Table_ptr = new Table2D(filename);
    Tb_length = EOS_Mu_Table_ptr->getTbsizeX();
    nStableParticles = EOS_Mu_Table_ptr->getTbsizeY() - 1;
    T = new double [Tb_length];
    mu_table = new double* [nStableParticles];
    for(int i = 0; i < nStableParticles; i++) mu_table[i] = new double [Tb_length];
    for(int i = 0; i < Tb_length; i++)
    {
        T[i] = EOS_Mu_Table_ptr->getTbdata(i, 0);
        for(int j = 0; j < nStableParticles; j++)
            mu_table[j][i] = EOS_Mu_Table_ptr->getTbdata(i, j+1);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get mu vs stable particle index for the given temperature index
//////////////////////////////////////////////////////////////////////////////////////////////////
void ChemicalPotential::getStableParticleMuTable(int iT, double* mu)
{
    for(int i = 0; i < nStableParticles; i++) mu[i] = mu_table[i][iT];
}

void ChemicalPotential::output_stable_mu(double temperature, double* mu)
{
    for(int i = 0; i < nStableParticles; i++)
        interpolation1D_linear(T, mu_table[i], &temperature, &mu[i], Tb_length);
}
