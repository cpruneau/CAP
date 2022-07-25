#ifndef CAP_ChemicalPotential
#define CAP_ChemicalPotential
#include "Table2D.hpp"

class ChemicalPotential
{
   private: 
      int Tb_length, nStableParticles;
      Table2D* EOS_Mu_Table_ptr;
      double* T;
      double T_chem;
      int nT;
      double** mu_table;
      int read_in_flag;
      int *stable_Particle_monval_list;
      
   public:
      ChemicalPotential();
      ~ChemicalPotential();

      void readChemicalPotentialTableFromFile(string filename);
      void readStableParticleListFromFile(string filename);

      int get_Tblength() {return(Tb_length);};
      int getNStableParticles() {return(nStableParticles);};
      int get_stable_Particle_monval(int i) 
          {return(stable_Particle_monval_list[i]);};
      double get_T(int i) {return(T[i]);};

      void getStableParticleMuTable(int iT, double* mu);
      void output_stable_mu(double temperature, double* mu); 

};


#endif
