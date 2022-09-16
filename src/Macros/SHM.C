void SHM()
{
//  TFile  * fg = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Gen_Derived.root");
//  TFile  * fr = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Reco_Derived.root");
//
//  TH2F * r2Gen  = (TH2F*) fg->Get("Pair_Gen_All_HP_HM_R2_DetaDphi_shft");
//  TH2F * r2Reco = (TH2F*) fr->Get("Pair_Reco_All_HP_HM_R2_DetaDphi_shft");

  int nPoints = 1000;

  TCanvas * c1 = new TCanvas("s1","s1",800,400);
  TF1 * sine1 = new TF1("s1","[0]+[1]*sin(x)",0.0,3.0*TMath::TwoPi());
  sine1->SetNpx(nPoints);
  sine1->SetParameter(0,0.0);
  sine1->SetParameter(1,1.0);
  sine1->SetTitle("y=sin(x)");
  sine1->GetXaxis()->SetTitle("x");
  sine1->GetXaxis()->SetLabelSize(0.05);
  sine1->GetXaxis()->SetTitleOffset(0.65);
  sine1->GetXaxis()->SetTitleSize(0.07);
  sine1->GetYaxis()->SetLabelSize(0.05);
  sine1->GetYaxis()->SetTitleOffset(0.65);
  sine1->GetYaxis()->SetTitleSize(0.07);
  sine1->GetYaxis()->SetTitle("y");
  sine1->Draw();
  c1->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s1.pdf");

  TCanvas * c2 = new TCanvas("s2","s2",800,400);
  TF1 * sine2 = new TF1("s2","[0]+[1]*sin(2*x)",0.0,3.0*TMath::TwoPi());
  sine2->SetNpx(nPoints);
  sine2->SetParameter(0,0.0);
  sine2->SetParameter(1,1.0);
  sine2->SetTitle("y=sin(2x)");
  sine2->GetXaxis()->SetTitle("x");
  sine2->GetXaxis()->SetLabelSize(0.05);
  sine2->GetXaxis()->SetTitleOffset(0.65);
  sine2->GetXaxis()->SetTitleSize(0.07);
  sine2->GetYaxis()->SetLabelSize(0.05);
  sine2->GetYaxis()->SetTitleOffset(0.65);
  sine2->GetYaxis()->SetTitleSize(0.07);
  sine2->GetYaxis()->SetTitle("y");
  sine2->Draw();
  c2->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s2.pdf");

  TCanvas * c3 = new TCanvas("s3","s3",800,400);
  TF1 * sine3 = new TF1("s3","[0]+[1]*sin(3*x)",0.0,3.0*TMath::TwoPi());
  sine3->SetNpx(nPoints);
  sine3->SetParameter(0,0.0);
  sine3->SetParameter(1,1.0);
  sine3->SetTitle("y=sin(3x)");
  sine3->GetXaxis()->SetTitle("x");

  sine3->GetXaxis()->SetLabelSize(0.05);
  sine3->GetXaxis()->SetTitleOffset(0.65);
  sine3->GetXaxis()->SetTitleSize(0.07);
  sine3->GetYaxis()->SetLabelSize(0.05);
  sine3->GetYaxis()->SetTitleOffset(0.65);
  sine3->GetYaxis()->SetTitleSize(0.07);
  sine3->GetYaxis()->SetTitle("y");
  sine3->Draw();
  c3->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s3.pdf");

  TCanvas * c4 = new TCanvas("s4","s4",800,400);
  TF1 * sine4 = new TF1("s4","[0]+[1]*sin(4*x)",0.0,3.0*TMath::TwoPi());
  sine4->SetNpx(nPoints);
  sine4->SetParameter(0,0.0);
  sine4->SetParameter(1,1.0);
  sine4->SetTitle("y=sin(4x)");
  sine4->GetXaxis()->SetTitle("x");
  sine4->GetXaxis()->SetLabelSize(0.05);
  sine4->GetXaxis()->SetTitleOffset(0.65);
  sine4->GetXaxis()->SetTitleSize(0.07);
  sine4->GetYaxis()->SetLabelSize(0.05);
  sine4->GetYaxis()->SetTitleOffset(0.65);
  sine4->GetYaxis()->SetTitleSize(0.07);
  sine4->GetYaxis()->SetTitle("y");
  sine4->Draw();
  c4->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s4.pdf");

  TCanvas * c5 = new TCanvas("s5","s5",800,400);
  TF1 * sine5 = new TF1("s5","[0]+[1]*sin(x)+[2]*sin(2*x)",0.0,3.0*TMath::TwoPi());
  sine5->SetNpx(nPoints);
  sine5->SetParameter(0,0.0);
  sine5->SetParameter(1,1.0);
  sine5->SetParameter(2,1.0);
  sine5->SetTitle("y=sin(x)+sin(2x)");
  sine5->GetXaxis()->SetTitle("x");
  sine5->GetXaxis()->SetLabelSize(0.05);
  sine5->GetXaxis()->SetTitleOffset(0.65);
  sine5->GetXaxis()->SetTitleSize(0.07);
  sine5->GetYaxis()->SetLabelSize(0.05);
  sine5->GetYaxis()->SetTitleOffset(0.65);
  sine5->GetYaxis()->SetTitleSize(0.07);
  sine5->GetYaxis()->SetTitle("y");
  sine5->Draw();
  c5->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s5.pdf");

  TCanvas * c6 = new TCanvas("s6","s6",800,400);
  TF1 * sine6 = new TF1("s6","[0]+[1]*sin(x)+[2]*sin(2*x)",0.0,3.0*TMath::TwoPi());
  sine6->SetNpx(nPoints);
  sine6->SetParameter(0,0.0);
  sine6->SetParameter(1,1.0);
  sine6->SetParameter(2,0.5);
  sine6->SetTitle("y=sin(x)+0.5*sin(2x)");
  sine6->GetXaxis()->SetTitle("x");
  sine6->GetXaxis()->SetLabelSize(0.05);
  sine6->GetXaxis()->SetTitleOffset(0.65);
  sine6->GetXaxis()->SetTitleSize(0.07);
  sine6->GetYaxis()->SetLabelSize(0.05);
  sine6->GetYaxis()->SetTitleOffset(0.65);
  sine6->GetYaxis()->SetTitleSize(0.07);
  sine6->GetYaxis()->SetTitle("y");
  sine6->Draw();
  c6->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s6.pdf");

  TCanvas * c7 = new TCanvas("s7","s7",800,400);
  TF1 * sine7 = new TF1("s7","[0]+[1]*sin(x)+[2]*sin(2*x)+[3]*sin(3*x)+[4]*sin(4*x)+[5]*sin(5*x)",0.0,3.0*TMath::TwoPi());
  sine7->SetNpx(nPoints);
  sine7->SetParameter(0,0.0);
  sine7->SetParameter(1,1.0);
  sine7->SetParameter(2,0.0);
  sine7->SetParameter(3,0.3333);
  sine7->SetParameter(4,0.0);
  sine7->SetParameter(5,0.2);
  sine7->SetTitle("y=sin(x)+sin(3x)/3+sin(5x)/5");
  sine7->GetXaxis()->SetTitle("x");
  sine7->GetXaxis()->SetLabelSize(0.05);
  sine7->GetXaxis()->SetTitleOffset(0.65);
  sine7->GetXaxis()->SetTitleSize(0.07);
  sine7->GetYaxis()->SetLabelSize(0.05);
  sine7->GetYaxis()->SetTitleOffset(0.65);
  sine7->GetYaxis()->SetTitleSize(0.07);
  sine7->GetYaxis()->SetTitle("y");
  sine7->Draw();
  c7->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s7.pdf");

  TCanvas * c8 = new TCanvas("s8","s8",800,400);
  TF1 * sine8 = new TF1("s8","[0]+[1]*sin(x)+[2]*sin(2*x)+[3]*sin(3*x)+[4]*sin(4*x)+[5]*sin(5*x)+[6]*sin(6*x)+[7]*sin(7*x)+[8]*sin(8*x)+[9]*sin(9*x)+[10]*sin(10*x)+[11]*sin(11*x)",0.0,3.0*TMath::TwoPi());
  sine8->SetNpx(nPoints);
  sine8->SetParameter(0,0.0);
  sine8->SetParameter(1,1.0);
  sine8->SetParameter(2,0.0);
  sine8->SetParameter(3,0.33333333);
  sine8->SetParameter(4,0.0);
  sine8->SetParameter(5,0.2);
  sine8->SetParameter(6,0.0);
  sine8->SetParameter(7,1.0/7.0);
  sine8->SetParameter(8,0.0);
  sine8->SetParameter(9,1.0/9.0);
  sine8->SetParameter(10,0.0);
  sine8->SetParameter(11,1.0/11.0);
  sine8->SetTitle("y=sin(x)+sin(3x)/3+sin(5x)/5+sin(7x)/7+sin(9x)/9+sin(11x)/11");
  sine8->GetXaxis()->SetTitle("x");
  sine8->GetXaxis()->SetLabelSize(0.05);
  sine8->GetXaxis()->SetTitleOffset(0.65);
  sine8->GetXaxis()->SetTitleSize(0.07);
  sine8->GetYaxis()->SetLabelSize(0.05);
  sine8->GetYaxis()->SetTitleOffset(0.65);
  sine8->GetYaxis()->SetTitleSize(0.07);
  sine8->GetYaxis()->SetTitle("y");
  sine8->Draw();
  c8->Print("/Users/aa7526/Documents/MyCourses/Phy3100/plots/s8.pdf");


  //
}
