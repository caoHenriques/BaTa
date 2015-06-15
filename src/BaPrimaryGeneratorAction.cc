
#include "BaPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "CLHEP/Units/PhysicalConstants.h"

#include <iostream>
#include <fstream>


using namespace CLHEP;

BaPrimaryGeneratorAction::BaPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0)
{

  G4String particleName ="e-";  // "opticalphoton"; //
  G4double wavelength= 493.54 * nm;   // blue light in BaTa
  G4double energy = h_Planck*c_light/wavelength;

  // initial orientation
  G4ThreeVector position= G4ThreeVector(0,0,0);
  G4ThreeVector momentumDirection = G4ThreeVector(0,0,0);

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(momentumDirection);
  fParticleGun->SetParticlePolarization(G4ThreeVector(1.,0.,0.));

    char filename[]="output.txt";
	FILE* file=fopen(filename, "w");
	fprintf(file, "%d	%d	%d	%d %d", 0,0,0,0,0);
	fclose(file);
	G4cout << "output.txt created "<< G4endl;

}

BaPrimaryGeneratorAction::~BaPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void BaPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
 
/*	
  G4double cosTheta = 2*G4UniformRand() - 1.,
	  phi = 2*3.141592*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi),
           uy = sinTheta*std::sin(phi),
           uz = cosTheta;
*/

  //this function is called at the begining of ecah event
  G4double sqrt2=0.70710678118;
  G4double RBhs= 0.06; // laser beam area, radius in m
  G4double HBs= 0.38; //altura, related with detector contruction
  G4double PBs= 5.55+HBs; 

  G4double pi=3.14159265359;
  G4double phi = 2*pi*G4UniformRand();
  G4double radius = G4UniformRand()* RBhs;

  //direction
  G4double Xq= 1.*sqrt2*std::sin(54.7356103*pi/180);
  G4double Yq= 1.*std::cos(54.7356103*pi/180);
  G4double Zq= Xq;
 

  //perpendicular to laser and x axis
  G4double Pnorm=sqrt(pow(Zq,2) * pow(Yq,2));
  G4double Xp=0.;
  G4double Yp=Zq/Pnorm;
  G4double Zp=-Yq/Pnorm;

 
  // vector rotation across beam direction from Rodrigues formula + random + initial position
  G4double xl=-PBs*Xq+ radius*(Xp*std::cos(phi) + std::sin(phi)*(Yq*Zp-Zq*Yp)\
	  + Xq*(1-std::cos(phi))*(Xq*Xp+Yq*Yp+Zq*Zp));

  G4double yl=-PBs*Yq+ radius*(Yp*std::cos(phi) + std::sin(phi)*(Zq*Xp-Xq*Zp)\
	  + Yq*(1-std::cos(phi))*(Xq*Xp+Yq*Yp+Zq*Zp));

  G4double zl=-PBs*Zq+ radius*(Zp*std::cos(phi) + std::sin(phi)*(Xq*Yp-Yq*Xp)\
	  + Zq*(1-std::cos(phi))*(Xq*Xp+Yq*Yp+Zq*Zp));

  fParticleGun->
	  SetParticlePosition(G4ThreeVector(xl*cm,yl*cm,zl*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(Xq,Yq,Zq));
  fParticleGun->SetParticlePolarization(G4ThreeVector(Xp,Yp,Zp));
  fParticleGun->GeneratePrimaryVertex(anEvent);
    
  
    // count photon and save
	G4int _countT=-1;
    G4int _countLB, _countWT, _countW1, _countW2;

	std::ifstream myfile;
	myfile.open ("output.txt");
	
	myfile >> _countT >> _countLB >> _countWT >> _countW1 >> _countW2;
	
	_countT++;	
	myfile.close();

	char filename[]="output.txt";
	FILE* file=fopen(filename, "w");
	fprintf(file, "%d	%d	%d	%d %d",\
		_countT,_countLB,_countWT,_countW1,_countW2);
	fclose(file);
	
	G4cout << "counT= "<< _countT << G4endl;

}

