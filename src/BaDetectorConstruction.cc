
#include "BaDetectorConstruction.hh"
#include "BaOpticalMaterialProperties.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"


BaDetectorConstruction::BaDetectorConstruction()
: G4VUserDetectorConstruction()
{ }

BaDetectorConstruction::~BaDetectorConstruction()
{ }

G4VPhysicalVolume* BaDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = false;

  //////////////    Materials    /////////////////////////
  G4Material* matChamber = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4double pressure= 10.*bar;
  G4double temperature= 298.15*kelvin;
  //G4double _sc_yield=16670. * 1/MeV;
  G4Material* matXe = nist->ConstructNewGasMaterial("HPXe","G4_Xe",temperature,pressure);
  matXe->SetMaterialPropertiesTable(BaOpticalMaterialProperties::GXe(pressure, temperature));




  ///////////////////////    shapes     //////////////////////


  // sphere   
  G4double Rmin=4.191;
  G4double Rmax=5.819;
  G4double RF1=1.9045;
  G4double Dcyl=Rmax+1.0;

  G4Sphere* sphere=new G4Sphere("Sphere", Rmin*cm, Rmax*cm, 0.*deg,
             360.*deg, 0.*deg, 180.*deg);
  



  //------ bigger flanges holes

  // hole cylinder
  G4Tubs* Flange1 = new G4Tubs("Flange1", 0.*cm, RF1*cm, Dcyl*cm,0.*deg,360.*deg);
  
  //sub 1
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  G4VSolid* sub =  new G4SubtractionSolid("SphereFlange1", sphere, Flange1,
	  rm, G4ThreeVector(0.,0.,0.));

  //sub 2
  rm->rotateX(90.*deg);
  sub =  new G4SubtractionSolid("SphereFlange2", sub, Flange1,
	  rm, G4ThreeVector(0.,0.,0.));
  
  //sub 3
  sub =  new G4SubtractionSolid("SphereFlange3", sub, Flange1,
	  0, G4ThreeVector(0.,0.,0.));



  //------- smaller flanges holes

  G4double sqrt3=0.57735026919;
  G4double sqrt2=0.70710678118;
  G4double RF2=0.81225;
  G4double aveRsqrt3=4*sqrt3;

  // hole cylinder
  G4Tubs* Flange2 = new G4Tubs("flange2", 0.*cm, RF2*cm, Dcyl*cm,0.*deg,360.*deg);

  // sub 1
  rm->setAxis(G4ThreeVector(sqrt2,sqrt2,0.0));
  rm->setDelta(-54.7356103 *deg);
  sub =  new G4SubtractionSolid("SphereFlange4", sub, Flange2,
	  rm, G4ThreeVector(0.0,0.0,0.0));

  // sub 2
  rm->setAxis(G4ThreeVector(sqrt2,sqrt2,0.0));
  rm->setDelta(54.7356103 *deg);
  sub =  new G4SubtractionSolid("SphereFlange5", sub, Flange2,
	  rm, G4ThreeVector(0.,0.,0.));
  
  // sub 3
  rm->setAxis(G4ThreeVector(-sqrt2,sqrt2,0.0));
  rm->setDelta(-54.7356103 *deg);
  sub =  new G4SubtractionSolid("SphereFlange6", sub, Flange2,
	  rm, G4ThreeVector(0.,0.,0.));
  
  // sub 4
  rm->setAxis(G4ThreeVector(sqrt2,-sqrt2,0.0));
  rm->setDelta(-54.7356103 *deg);
  sub =  new G4SubtractionSolid("SphereFlange7", sub, Flange2,
	  rm, G4ThreeVector(0.,0.,0.));
	  
  
  //------------- blanks
  // reset rm
  rm->setAxis(G4ThreeVector(0.,1.,0.0));
  rm->setDelta(54.7356103*deg);
  rm->rotateY((90.-54.7356103)*deg);
  rm->rotateX(90.*deg);

  //>>>> Bigger blanks

  // blank cylinder
  G4double RB= 6.934/2;
  G4double HB= 0.7;
  G4double PB= 9.347/2+HB;
  G4Tubs* Blank1 = new G4Tubs("blank1", 0.*cm, RB*cm, HB*cm,0.*deg,360.*deg);

  // b1
  sub =  new G4UnionSolid("SphereBlank1", sub, Blank1,
	  rm, G4ThreeVector(0.,PB*cm,0.));

  // b2
  sub =  new G4UnionSolid("SphereBlank2", sub, Blank1,
	  rm, G4ThreeVector(0.,-PB*cm,0.));

  // b2
  rm->rotateY(90.*deg);
  sub =  new G4UnionSolid("SphereBlank3", sub, Blank1,
	  rm, G4ThreeVector(0.,0.,PB*cm));

  
  // Vacuum pipe
  G4double HBp= 2.;
  G4double PBp= 9.34/2+HBp; //9.347/2+HBp;
  G4Tubs* Pipe1 = new G4Tubs("blank1_p", (RF1+0.1)*cm, (RB-0.5)*cm, HBp*cm,0.*deg,360.*deg);

  rm->setAxis(G4ThreeVector(0.,1.,0.));
  rm->setDelta(90.*deg);
  sub =  new G4UnionSolid("SpherePipe1", sub, Pipe1,
	  rm, G4ThreeVector(-PBp*cm,0.,0.));
  
  // vacumm pipe blank
  G4Tubs* Blank_p = new G4Tubs("blank_p", 0.*cm, (RB-0.49)*cm, (HB-0.3)*cm,0.*deg,360.*deg);

  sub =  new G4UnionSolid("SpherePipe2", sub, Blank_p,
	  rm, G4ThreeVector((-PBp-HBp+0.1)*cm,0.,0.));
	  

  //>>>>> Smaller blanks

  // blank cylinder
  G4double RBs= 3.38/2; 
  G4double HBs= 0.5; //1.;
  G4double PBs= 5.55+HBs; //5.569+HBs;
  G4Tubs* Blank2 = new G4Tubs("blank2", 0.*cm, RBs*cm, HBs*cm,0.*deg,360.*deg);

  // localization
  G4double Yq= PBs*cos(54.7356103*deg);
  G4double Xq= PBs*sqrt2*sin(54.7356103*deg);
  G4double Zq= Xq;

  // b1
  rm->setAxis(G4ThreeVector(sqrt2,sqrt2, 0.0));
  rm->setDelta(-54.7356103 *deg);
  sub =  new G4UnionSolid("SphereBlankS1", sub, Blank2,
	  rm, G4ThreeVector(-Xq*cm, Yq*cm, -Zq*cm)); 
  
  // b2
  sub =  new G4UnionSolid("SphereBlankS2", sub, Blank2,
	  rm, G4ThreeVector(Xq*cm, -Yq*cm, Zq*cm));


  
  // bL - laser blank with hole
  G4double RBhs= 0.3; // laser hole
  G4Tubs* Blank3 = new G4Tubs("blank3", RBhs*cm, RBs*cm, HBs*cm,0.*deg,360.*deg);

  // bL_1
  rm->setAxis(G4ThreeVector(sqrt2, -sqrt2, 0.));
  rm->setDelta(54.7356103 *deg);
  sub =  new G4UnionSolid("SphereBlankS3", sub, Blank3,
	  rm, G4ThreeVector(-Xq*cm, -Yq*cm, -Zq*cm));
  
  // bL_2
  sub =  new G4UnionSolid("SphereBlankS4", sub, Blank3,
	  rm, G4ThreeVector(Xq*cm, Yq*cm, Zq*cm));


  // b3
  rm->setAxis(G4ThreeVector(sqrt2, sqrt2, 0.));
  rm->setDelta(54.7356103 *deg);
  sub =  new G4UnionSolid("SphereBlankS5", sub, Blank2,
	  rm, G4ThreeVector(-Xq*cm, Yq*cm, Zq*cm));

  // b4 
  sub =  new G4UnionSolid("SphereBlankS6", sub, Blank2,
	  rm, G4ThreeVector(Xq*cm, -Yq*cm, -Zq*cm));

  // b4 
  rm->setAxis(G4ThreeVector(-sqrt2, sqrt2, 0.));
  rm->setDelta(54.7356103 *deg);
  sub =  new G4UnionSolid("SphereBlankS7", sub, Blank2,
	  rm, G4ThreeVector(Xq*cm, Yq*cm, -Zq*cm));

  // b5
  sub =  new G4UnionSolid("SphereBlankS8", sub, Blank2,
	  rm, G4ThreeVector(-Xq*cm, -Yq*cm, Zq*cm));

  

  //---------- detectors shapes

  //laser detector
  G4Tubs* laser_detector =
	  new G4Tubs("laser_detector", 0.*cm, RBhs*cm, 0.1*cm,0.*deg,360.*deg);

  //window photosensor1  
  G4Tubs* photosensor1 =
	  new G4Tubs("photosensor1", 0.*cm, RF1*cm, 0.15*cm,0.*deg,360.*deg);





  //----------  Xenon volume shape (subtractions)

  //box to be subtracted
  G4double side_Xebox=9.5;
  G4Box* boXe = new G4Box("Xebox", side_Xebox*cm, side_Xebox*cm, side_Xebox*cm);

  //>>>>  subtract
  //the detector
  G4VSolid* subXe =  new G4SubtractionSolid("subXe1", boXe, sub,
	  0, G4ThreeVector(0.,0.,0.));

  //subtract the laser sensor
  G4RotationMatrix* rm3 = 
	  new G4RotationMatrix(G4ThreeVector(sqrt2,-sqrt2,0.),54.7356103 *deg);

  subXe =  new G4SubtractionSolid("subXe2", subXe, laser_detector,
	  rm3, G4ThreeVector(Xq*cm, Yq*cm, Zq*cm));


  //subtract the laser exit
  subXe =  new G4SubtractionSolid("subXe3", subXe, laser_detector,
	  rm3, G4ThreeVector(-Xq*cm, -Yq*cm, -Zq*cm));



  //subtract windows
  G4double center_distance_ps=Rmin + 0.4;
  G4double Xps=center_distance_ps*sqrt2*sin(54.7356103*deg);
  G4double Yps= center_distance_ps*cos(54.7356103*deg);
  G4double Zps= Xps;

  G4RotationMatrix* rm2 = new G4RotationMatrix();
  rm2->rotateY(90.*deg);

  subXe =  new G4SubtractionSolid("subXe4", subXe, photosensor1,
	  0, G4ThreeVector(0., 0., -center_distance_ps*cm));

  subXe =  new G4SubtractionSolid("subXe5", subXe, photosensor1,
	  rm2, G4ThreeVector(center_distance_ps*cm, 0., 0.));








  ////////////////////////////// logic shapes /////////////////////////////////////////////////////
  

  //----- World
  G4double world_size = 10.*cm;               //8.2*cm;

  G4Box* solidWorld =    
    new G4Box("World",                               //its name
       (world_size+1.), world_size, world_size);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        matXe,               //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
           




  //----- laser detector
  G4LogicalVolume* laser_detector_log =                         
	new G4LogicalVolume(laser_detector,    //its solid
                        matChamber,          //its material
                        "laser_detector");          //its name

  G4VPhysicalVolume* laser_detector_phy = 
	new G4PVPlacement(rm3,                       //no rotation
                    G4ThreeVector(Xq*cm, Yq*cm, Zq*cm),    //at position
                    laser_detector_log,                 //its logical volume
                    "laser_detector",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 

  //----- laser exit
  G4LogicalVolume* laser_exit_log =                         
	new G4LogicalVolume(laser_detector,    //its solid
                        matChamber,          //its material
                        "laser_exit");          //its name

  G4VPhysicalVolume* laser_exit_phy = 
	new G4PVPlacement(rm3,                       //no rotation
                    G4ThreeVector(-Xq*cm, -Yq*cm, -Zq*cm),    //at position
                    laser_exit_log,                 //its logical volume
                    "laser_exit",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 




  //----- chamber

  G4LogicalVolume* chamber_log =                         
  new G4LogicalVolume(sub,                   //its solid
                        matChamber,          //its material
                        "Chamber");           //its name
  
  G4VPhysicalVolume* chamber_phy = 
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, 0),          //at position
                    chamber_log,                 //its logical volume
                    "Chamber",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  
  //----- window photosensor 1

  G4LogicalVolume* photosensor1_log =                         
	new G4LogicalVolume(photosensor1,    //its solid
                        matChamber,          //its material
                        "photosensor1");          //its name

  G4VPhysicalVolume* photosensor1_phy = 
	new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0., 0., -center_distance_ps*cm),                    //at position
                    photosensor1_log,                 //its logical volume
                    "photosensor1",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 




  //------ window photosensor 2

  G4LogicalVolume* photosensor2_log =                         
	new G4LogicalVolume(photosensor1,    //its solid
                        matChamber,          //its material
                        "photosensor2");          //its name

  G4VPhysicalVolume* photosensor2_phy = 
	new G4PVPlacement(rm2,                       //no rotation
                    G4ThreeVector(center_distance_ps*cm, 0., 0.),                    //at position
                    photosensor2_log,                 //its logical volume
                    "photosensor2",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 



 
  //----- Xe logic

  G4LogicalVolume* volXe_log =                         
  new G4LogicalVolume(subXe,                   //its solid
                        matXe,                 //its material
                        "Xenon_Volume");       //its name

  G4VPhysicalVolume* volXe_phy =          
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, 0),  //at position
                    volXe_log,                   //its logical volume
                    "Xenon_Volume",          //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking





  ///////////////////// visualization //////////////////////////////////
  
  // chamber
  G4VisAttributes* shape1VisAtt1
    = new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.1));
  shape1VisAtt1->SetForceWireframe(true);
  chamber_log->SetVisAttributes(shape1VisAtt1);

  // laser detector
  G4VisAttributes* shape1VisAtt2
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.2));
  shape1VisAtt2->SetForceWireframe(true);
  laser_detector_log->SetVisAttributes(shape1VisAtt2);

  // Xe invisible
  G4VisAttributes* shape1VisAtt3
    = new G4VisAttributes(false);
  volXe_log->SetVisAttributes(shape1VisAtt3);
  
  // photosensor1
  G4VisAttributes* shape1VisAtt4
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  shape1VisAtt4->SetForceWireframe(true);
  photosensor1_log->SetVisAttributes(shape1VisAtt4);
 
  // photosensor2
  G4VisAttributes* shape1VisAtt5
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  shape1VisAtt5->SetForceWireframe(true);
  photosensor2_log->SetVisAttributes(shape1VisAtt5);
   
  // laser exit
  G4VisAttributes* shape1VisAtt6
    = new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.5));
  shape1VisAtt6->SetForceWireframe(true);
  laser_exit_log->SetVisAttributes(shape1VisAtt6);





  ////////////////////  optical surfaces  //////////////////////////

  //----- stainless steal reflection
  const G4int entries_R = 2;
  G4double ENERGIES_R[entries_R]={1.*eV, 10.*eV};
  G4double REFLECTIVITY[entries_R]={0.55, 0.55};   //->> confirm!!!!!

  G4MaterialPropertiesTable* inner_face = new G4MaterialPropertiesTable();
  inner_face->AddProperty("REFLECTIVITY", ENERGIES_R, REFLECTIVITY, entries_R);
     
  G4OpticalSurface* iface_opsur =
		new G4OpticalSurface("POLISH", unified, polished, dielectric_metal);
  iface_opsur->SetMaterialPropertiesTable(inner_face);

  G4LogicalSkinSurface * AirSurface = new G4LogicalSkinSurface("chamber_surface",
	  chamber_log,iface_opsur);


 //----- laser exit optical
  G4double REFLECTIVITY3[entries_R]={0.0, 0.0};   

  G4MaterialPropertiesTable* laser_exit_opto = new G4MaterialPropertiesTable();
  laser_exit_opto->AddProperty("REFLECTIVITY", ENERGIES_R, REFLECTIVITY3, entries_R);
     
  G4OpticalSurface* laser_exit_opsur =
		new G4OpticalSurface("POLISH", unified, polished, dielectric_metal);
  laser_exit_opsur->SetMaterialPropertiesTable(laser_exit_opto);

  G4LogicalSkinSurface * laserSurface = new G4LogicalSkinSurface(\
	  "laser_exit_surface",  laser_exit_log, laser_exit_opsur);


  //----- photosensors efficiency
    G4double EFFICIENCY[entries_R]={1.0, 1.0};
	G4double REFLECTIVITY_2[entries_R]={0.0, 0.0};

	G4MaterialPropertiesTable* window1 = new G4MaterialPropertiesTable();
    window1->AddProperty("EFFICIENCY", ENERGIES_R, EFFICIENCY, entries_R);
	window1->AddProperty("REFLECTIVITY", ENERGIES_R, REFLECTIVITY_2, entries_R);

	G4OpticalSurface* window1_surf =
		new G4OpticalSurface("POLISH", unified, polished, dielectric_metal);
    window1_surf->SetMaterialPropertiesTable(window1);

	G4LogicalSkinSurface * window_sensor1
	    = new G4LogicalSkinSurface("window_sensor1", laser_detector_log, window1_surf);

	G4LogicalSkinSurface * window_sensor2
		= new G4LogicalSkinSurface("window_sensor2", photosensor1_log, window1_surf);
		
	G4LogicalSkinSurface * window_sensor3
		= new G4LogicalSkinSurface("window_sensor3", photosensor2_log, window1_surf);


  return physWorld;
}
