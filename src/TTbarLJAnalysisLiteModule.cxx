#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/MCWeight.h>
#include <UHH2/common/include/CleaningModules.h>
#include <UHH2/common/include/NSelections.h>
#include <UHH2/common/include/LumiSelection.h>
#include <UHH2/common/include/TriggerSelection.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/JetIds.h>
#include <UHH2/common/include/TopJetIds.h>
#include <UHH2/common/include/TTbarGen.h>
#include <UHH2/common/include/TTbarReconstruction.h>
#include <UHH2/common/include/ReconstructionHypothesisDiscriminators.h>
#include <UHH2/common/include/Utils.h>

#include <UHH2/ZprimeSemiLeptonic/include/ModuleBASE.h>
#include <UHH2/ZprimeSemiLeptonic/include/ZprimeSemiLeptonicSelections.h>
#include <UHH2/ZprimeSemiLeptonic/include/ZprimeSemiLeptonicUtils.h>
#include <UHH2/ZprimeSemiLeptonic/include/TTbarLJHists.h>
#include <UHH2/ZprimeSemiLeptonic/include/DileptonHists.h>
#include <UHH2/ZprimeSemiLeptonic/include/EffyTTbarRECOHists.h>

#include <UHH2/ZprimeSemiLeptonic/include/SF_pileup.h>
#include <UHH2/ZprimeSemiLeptonic/include/SF_btagging.h>
#include <UHH2/ZprimeSemiLeptonic/include/SF_ttagging.h>

class TTbarLJAnalysisLiteModule : public ModuleBASE {

 public:
  explicit TTbarLJAnalysisLiteModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

 protected:
  enum lepton { muon, elec };
  lepton channel_;

  // selections
  std::unique_ptr<uhh2::Selection> jet2_sel;
  std::unique_ptr<uhh2::Selection> jet1_sel;
  std::unique_ptr<uhh2::Selection> trigger_sel;
  std::unique_ptr<uhh2::Selection> met_sel;
  std::unique_ptr<uhh2::Selection> htlep_sel;
  std::unique_ptr<uhh2::Selection> triangc_sel;
  std::unique_ptr<uhh2::Selection> ttagevt_sel;
  std::unique_ptr<uhh2::Selection> topleppt_sel;
  std::unique_ptr<uhh2::Selection> chi2_sel;

  // ttbar reconstruction
  std::unique_ptr<uhh2::AnalysisModule> ttgenprod;
  std::unique_ptr<uhh2::AnalysisModule> reco_primlep;
  std::unique_ptr<uhh2::AnalysisModule> ttbar_reco__ttag0, ttbar_reco__ttag1;
  std::unique_ptr<uhh2::AnalysisModule> ttbar_chi2__ttag0, ttbar_chi2__ttag1;

  uhh2::Event::Handle<std::vector<ReconstructionHypothesis>> h_ttbar_hyps;

  bool use_ttagging_;
  TopJetId ttag_ID_;
  float    ttag_minDR_jet_;

  bool blind_DATA_;

  //// Data/MC scale factors
  std::unique_ptr<weightcalc_pileup> pileupSF_ct;
  std::unique_ptr<weightcalc_pileup> pileupSF_up;
  std::unique_ptr<weightcalc_pileup> pileupSF_dn;

  std::unique_ptr<weightcalc_btagging> btagSF_ct;
  std::unique_ptr<weightcalc_btagging> btagSF_upL;
  std::unique_ptr<weightcalc_btagging> btagSF_dnL;
  std::unique_ptr<weightcalc_btagging> btagSF_upB;
  std::unique_ptr<weightcalc_btagging> btagSF_dnB;

  std::unique_ptr<weightcalc_ttagging> ttagSF_ct;
  std::unique_ptr<weightcalc_ttagging> ttagSF_upL;
  std::unique_ptr<weightcalc_ttagging> ttagSF_dnL;
  std::unique_ptr<weightcalc_ttagging> ttagSF_upT;
  std::unique_ptr<weightcalc_ttagging> ttagSF_dnT;
  ////

  //// VARS

  // event
  Event::Handle<int>   h_run;
  Event::Handle<int>   h_lumi_block;
  Event::Handle<int>   h_event;
  Event::Handle<int>   h_pvN;
  Event::Handle<int>   h_muoN;
  Event::Handle<int>   h_eleN;
  Event::Handle<int>   h_btagN;
  Event::Handle<int>   h_ttagN;
  Event::Handle<bool>  h_ttagevt;
  //

  // lepton
  Event::Handle<TLorentzVector> h_lep1;
  Event::Handle<int>            h_lep1__pdgID;
  Event::Handle<int>            h_lep1__charge;
  Event::Handle<float>          h_lep1__minDR_jet;
  Event::Handle<float>          h_lep1__pTrel_jet;

  Event::Handle<TLorentzVector> h_lep2;
  Event::Handle<int>            h_lep2__pdgID;
  Event::Handle<int>            h_lep2__charge;
  Event::Handle<float>          h_lep2__minDR_jet;
  Event::Handle<float>          h_lep2__pTrel_jet;
  //

  // MET
  Event::Handle<TLorentzVector> h_MET;
  Event::Handle<float>          h_MET__pz;
  //

  // ttbar reco hyp
  Event::Handle<float>          h_rec_chi2;
  Event::Handle<TLorentzVector> h_rec_tlep;
  Event::Handle<TLorentzVector> h_rec_thad;
  //

  // weight
  Event::Handle<float> h_wgtMC__GEN;

  Event::Handle<float> h_wgtMC__pileupSF_ct;
  Event::Handle<float> h_wgtMC__pileupSF_up;
  Event::Handle<float> h_wgtMC__pileupSF_dn;

  Event::Handle<float> h_wgtMC__btagSF_ct;
  Event::Handle<float> h_wgtMC__btagSF_upL;
  Event::Handle<float> h_wgtMC__btagSF_dnL;
  Event::Handle<float> h_wgtMC__btagSF_upB;
  Event::Handle<float> h_wgtMC__btagSF_dnB;

  Event::Handle<float> h_wgtMC__ttagSF_ct;
  Event::Handle<float> h_wgtMC__ttagSF_upL;
  Event::Handle<float> h_wgtMC__ttagSF_dnL;
  Event::Handle<float> h_wgtMC__ttagSF_upT;
  Event::Handle<float> h_wgtMC__ttagSF_dnT;

  Event::Handle<float> h_wgtMC__muR_ct__muF_up;
  Event::Handle<float> h_wgtMC__muR_ct__muF_dn;
  Event::Handle<float> h_wgtMC__muR_up__muF_ct;
  Event::Handle<float> h_wgtMC__muR_up__muF_up;
  Event::Handle<float> h_wgtMC__muR_dn__muF_ct;
  Event::Handle<float> h_wgtMC__muR_dn__muF_dn;
  //

  ////
};

TTbarLJAnalysisLiteModule::TTbarLJAnalysisLiteModule(uhh2::Context& ctx){

  //// CONFIGURATION

  const bool isMC = (ctx.get("dataset_type") == "MC");

  const std::string& channel = ctx.get("channel", "");
  if     (channel == "muon") channel_ = muon;
  else if(channel == "elec") channel_ = elec;
  else throw std::runtime_error("TTbarLJAnalysisLiteModule::TTbarLJAnalysisLiteModule -- undefined argument for 'channel' key in xml file (must be 'muon' or 'elec'): "+channel);

  use_ttagging_ = true;

  blind_DATA_ = ((ctx.get("dataset_version").find("BLINDED") != std::string::npos) && (ctx.get("dataset_type") == "DATA") && !isMC);
  //

  const std::string& keyword = ctx.get("keyword");

  float jet1_pt(-1.), jet2_pt(-1.), MET(-1.), HT_lep(-1.);
  bool triangul_cut(false);
  bool topleppt_cut(false);

  if(keyword == "T0_v01" || keyword == "T1_v01"){

    if     (keyword == "T0_v01") use_ttagging_ = false;
    else if(keyword == "T1_v01") use_ttagging_ = true;

    if(channel_ == muon){

      jet1_pt = 150.;
      jet2_pt =  50.;

      MET    =  50.;
      HT_lep = 150.;

      triangul_cut = false;
      topleppt_cut = false;
    }
    else if(channel_ == elec){

      jet1_pt = 250.;
      jet2_pt =  70.;

      MET    = 120.;
      HT_lep =   0.;

      triangul_cut = false;
      topleppt_cut = false;
    }
  }
  else throw std::runtime_error("TTbarLJAnalysisLiteModule::TTbarLJAnalysisLiteModule -- undefined \"keyword\" argument in .xml configuration file: "+keyword);
  //

  ////

  //// COMMON MODULES
  ////

  //// OBJ CLEANING
  ////

  //// EVENT SELECTION
  jet2_sel.reset(new NJetSelection(2, -1, JetId(PtEtaCut(jet2_pt, 2.4))));
  jet1_sel.reset(new NJetSelection(1, -1, JetId(PtEtaCut(jet1_pt, 2.4))));

  const std::string& trigger = ctx.get("trigger", "NULL");
  if(trigger != "NULL") trigger_sel.reset(new TriggerSelection(trigger));
  else                  trigger_sel.reset(new uhh2::AndSelection(ctx));

  met_sel  .reset(new METCut  (MET   , uhh2::infinity));
  htlep_sel.reset(new HTlepCut(HT_lep, uhh2::infinity));

  if(triangul_cut){

    if     (channel_ == elec) triangc_sel.reset(new TriangularCuts(M_PI/2., (M_PI/2.)/75.));
    else if(channel_ == muon) triangc_sel.reset(new uhh2::AndSelection(ctx));
  }
  else triangc_sel.reset(new uhh2::AndSelection(ctx));
  ////

  //// TTBAR KINEMATICAL RECO

  /* t-tagging */
  ttag_ID_ = TopTagID_SoftDrop("mr003_wp1");
  ttag_minDR_jet_ = 1.2;

  ttagevt_sel.reset(new TopTagEventSelection(ttag_ID_, ttag_minDR_jet_));
  /*************/

  const std::string ttbar_gen_label ("ttbargen");
  const std::string ttbar_hyps_label("TTbarReconstruction");
  const std::string ttbar_chi2_label("Chi2");

  ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));

  reco_primlep.reset(new PrimaryLepton(ctx));

  ttbar_reco__ttag0.reset(new HighMassTTbarReconstruction(ctx, NeutrinoReconstruction, ttbar_hyps_label));
  ttbar_reco__ttag1.reset(new        TopTagReconstruction(ctx, NeutrinoReconstruction, ttbar_hyps_label, ttag_ID_, ttag_minDR_jet_));

  ttbar_chi2__ttag0.reset(new Chi2Discriminator    (ctx, ttbar_hyps_label));
  ttbar_chi2__ttag1.reset(new Chi2DiscriminatorTTAG(ctx, ttbar_hyps_label));

  h_ttbar_hyps = ctx.get_handle<std::vector<ReconstructionHypothesis>>(ttbar_hyps_label);
  ////

  if(topleppt_cut){

    if     (channel_ == elec) topleppt_sel.reset(new LeptonicTopPtCut(ctx, 140., uhh2::infinity, ttbar_hyps_label, ttbar_chi2_label));
    else if(channel_ == muon) topleppt_sel.reset(new uhh2::AndSelection(ctx));
  }
  else topleppt_sel.reset(new uhh2::AndSelection(ctx));

  chi2_sel.reset(new HypothesisDiscriminatorCut(ctx,  0., 30., ttbar_hyps_label, ttbar_chi2_label));

  //// HISTS

  std::vector<std::string> htags_1({

    "jet2",
    "jet1",
    "trigger",
    "met",
    "htlep",
    "triangc",
    "topleppt",
  });

  for(const auto& tag : htags_1){

    book_HFolder(tag, new TTbarLJHists(ctx, tag, ttag_ID_, ttag_minDR_jet_));
  }
  //

  std::vector<std::string> htags_2({

    "kine",
    "kine__t0b0",
    "kine__t0b1",
    "kine__t0b2",
    "kine__t1b0",
    "kine__t1b1",
    "kine__t1b2",

    "antichi2",
    "antichi2__t0b0",
    "antichi2__t0b1",
    "antichi2__t0b2",
    "antichi2__t1b0",
    "antichi2__t1b1",
    "antichi2__t1b2",

    "chi2",
    "chi2__t0b0",
    "chi2__t0b1",
    "chi2__t0b2",
    "chi2__t1b0",
    "chi2__t1b1",
    "chi2__t1b2",
  });

  for(const auto& tag : htags_2){

    book_HFolder(tag          , new TTbarLJHists      (ctx, tag          , ttag_ID_, ttag_minDR_jet_));
    book_HFolder(tag+"__ttbar", new EffyTTbarRECOHists(ctx, tag+"__ttbar", ttbar_gen_label, ttbar_hyps_label, ttbar_chi2_label));
  }
  //

  std::vector<std::string> htags_3({

    "chi2_L2",
    "chi2_L2__t0b0",
    "chi2_L2__t0b1",
    "chi2_L2__t0b2",
    "chi2_L2__t1b0",
    "chi2_L2__t1b1",
    "chi2_L2__t1b2",
  });

  for(const auto& tag : htags_3){

    book_HFolder(tag          , new TTbarLJHists      (ctx, tag          , ttag_ID_, ttag_minDR_jet_));
    book_HFolder(tag+"__dilep", new DileptonHists     (ctx, tag+"__dilep"));
    book_HFolder(tag+"__ttbar", new EffyTTbarRECOHists(ctx, tag+"__ttbar", ttbar_gen_label, ttbar_hyps_label, ttbar_chi2_label));
  }
  //

  ////

  //// Data/MC scale factors

  // pileup
  const std::string& pileup_MC      = ctx.get("pileup_MC");
  const std::string& pileup_DATA_ct = ctx.get("pileup_DATA_ct");
  const std::string& pileup_DATA_up = ctx.get("pileup_DATA_up");
  const std::string& pileup_DATA_dn = ctx.get("pileup_DATA_dn");

  pileupSF_ct.reset(new weightcalc_pileup(pileup_DATA_ct, pileup_MC, "pileup", "pileup"));
  pileupSF_up.reset(new weightcalc_pileup(pileup_DATA_up, pileup_MC, "pileup", "pileup"));
  pileupSF_dn.reset(new weightcalc_pileup(pileup_DATA_dn, pileup_MC, "pileup", "pileup"));
  //

  // b-tagging
  const std::string& btag_wp        = ctx.get("btag_wp");
  const std::string& btag_SFac_file = ctx.get("btag_SFs");
  const std::string& btag_effy_file = ctx.get("btag_eff");
  const std::string& btag_effyL     = ctx.get("btag_eff__jetL");
  const std::string& btag_effyC     = ctx.get("btag_eff__jetC");
  const std::string& btag_effyB     = ctx.get("btag_eff__jetB");

  btagSF_ct .reset(new weightcalc_btagging(btag_SFac_file, btag_wp, "comb", "mujets", "mujets", "CT", "CT", "CT", btag_effy_file, btag_effyL, btag_effyC, btag_effyB));

  btagSF_upL.reset(new weightcalc_btagging(btag_SFac_file, btag_wp, "comb", "mujets", "mujets", "UP", "CT", "CT", btag_effy_file, btag_effyL, btag_effyC, btag_effyB));
  btagSF_dnL.reset(new weightcalc_btagging(btag_SFac_file, btag_wp, "comb", "mujets", "mujets", "DN", "CT", "CT", btag_effy_file, btag_effyL, btag_effyC, btag_effyB));

  btagSF_upB.reset(new weightcalc_btagging(btag_SFac_file, btag_wp, "comb", "mujets", "mujets", "CT", "UP", "UP", btag_effy_file, btag_effyL, btag_effyC, btag_effyB));
  btagSF_dnB.reset(new weightcalc_btagging(btag_SFac_file, btag_wp, "comb", "mujets", "mujets", "CT", "DN", "DN", btag_effy_file, btag_effyL, btag_effyC, btag_effyB));
  //

  // t-tagging
  const std::string& ttag_wp        = ctx.get("ttag_wp");
  const std::string& ttag_SFac_file = ctx.get("ttag_SFs");
  const std::string& ttag_effy_file = ctx.get("ttag_eff");
  const std::string& ttag_effyL     = ctx.get("ttag_eff__jetL");
  const std::string& ttag_effyT     = ctx.get("ttag_eff__jetT");

  ttagSF_ct .reset(new weightcalc_ttagging(ttag_SFac_file, ttag_wp, "muon", "muon", "CT", "CT", ttag_effy_file, ttag_effyL, ttag_effyT));

  ttagSF_upL.reset(new weightcalc_ttagging(ttag_SFac_file, ttag_wp, "muon", "muon", "UP", "CT", ttag_effy_file, ttag_effyL, ttag_effyT));
  ttagSF_dnL.reset(new weightcalc_ttagging(ttag_SFac_file, ttag_wp, "muon", "muon", "DN", "CT", ttag_effy_file, ttag_effyL, ttag_effyT));

  ttagSF_upT.reset(new weightcalc_ttagging(ttag_SFac_file, ttag_wp, "muon", "muon", "CT", "UP", ttag_effy_file, ttag_effyL, ttag_effyT));
  ttagSF_dnT.reset(new weightcalc_ttagging(ttag_SFac_file, ttag_wp, "muon", "muon", "CT", "DN", ttag_effy_file, ttag_effyL, ttag_effyT));
  //

  ////

  //// VARS
  ctx.undeclare_all_event_output();

  // event
  h_run                = ctx.declare_event_output<int>("run");
  h_lumi_block         = ctx.declare_event_output<int>("lumi_block");
  h_event              = ctx.declare_event_output<int>("event");

  h_pvN                = ctx.declare_event_output<int>("pvN");

  h_muoN               = ctx.declare_event_output<int>("muoN");
  h_eleN               = ctx.declare_event_output<int>("eleN");

  h_btagN              = ctx.declare_event_output<int>("btagN");
  h_ttagN              = ctx.declare_event_output<int>("ttagN");

  h_ttagevt            = ctx.declare_event_output<bool>("ttagevt");
  //

  // lepton
  h_lep1               = ctx.declare_event_output<TLorentzVector>("lep1");
  h_lep1__pdgID        = ctx.declare_event_output<int>           ("lep1__pdgID");
  h_lep1__charge       = ctx.declare_event_output<int>           ("lep1__charge");
  h_lep1__minDR_jet    = ctx.declare_event_output<float>         ("lep1__minDR_jet");
  h_lep1__pTrel_jet    = ctx.declare_event_output<float>         ("lep1__pTrel_jet");

  h_lep2               = ctx.declare_event_output<TLorentzVector>("lep2");
  h_lep2__pdgID        = ctx.declare_event_output<int>           ("lep2__pdgID");
  h_lep2__charge       = ctx.declare_event_output<int>           ("lep2__charge");
  h_lep2__minDR_jet    = ctx.declare_event_output<float>         ("lep2__minDR_jet");
  h_lep2__pTrel_jet    = ctx.declare_event_output<float>         ("lep2__pTrel_jet");
  //

  // MET
  h_MET                = ctx.declare_event_output<TLorentzVector>("MET");
  h_MET__pz            = ctx.declare_event_output<float>         ("MET__pz");
  //

  // ttbar reco hyp
  h_rec_chi2           = ctx.declare_event_output<float>         ("rec_chi2");
  h_rec_tlep           = ctx.declare_event_output<TLorentzVector>("rec_tlep");
  h_rec_thad           = ctx.declare_event_output<TLorentzVector>("rec_thad");
  //

  // weight
  h_wgtMC__GEN         = ctx.declare_event_output<float>("wgtMC__GEN");

  h_wgtMC__pileupSF_ct = ctx.declare_event_output<float>("wgtMC__pileupSF_ct");
  h_wgtMC__pileupSF_up = ctx.declare_event_output<float>("wgtMC__pileupSF_up");
  h_wgtMC__pileupSF_dn = ctx.declare_event_output<float>("wgtMC__pileupSF_dn");

  h_wgtMC__btagSF_ct   = ctx.declare_event_output<float>("wgtMC__btagSF_ct");
  h_wgtMC__btagSF_upL  = ctx.declare_event_output<float>("wgtMC__btagSF_upL");
  h_wgtMC__btagSF_dnL  = ctx.declare_event_output<float>("wgtMC__btagSF_dnL");
  h_wgtMC__btagSF_upB  = ctx.declare_event_output<float>("wgtMC__btagSF_upB");
  h_wgtMC__btagSF_dnB  = ctx.declare_event_output<float>("wgtMC__btagSF_dnB");

  h_wgtMC__ttagSF_ct   = ctx.declare_event_output<float>("wgtMC__ttagSF_ct");
  h_wgtMC__ttagSF_upL  = ctx.declare_event_output<float>("wgtMC__ttagSF_upL");
  h_wgtMC__ttagSF_dnL  = ctx.declare_event_output<float>("wgtMC__ttagSF_dnL");
  h_wgtMC__ttagSF_upT  = ctx.declare_event_output<float>("wgtMC__ttagSF_upT");
  h_wgtMC__ttagSF_dnT  = ctx.declare_event_output<float>("wgtMC__ttagSF_dnT");

  h_wgtMC__muR_ct__muF_up = ctx.declare_event_output<float>("wgtMC__muR_ct__muF_up");
  h_wgtMC__muR_ct__muF_dn = ctx.declare_event_output<float>("wgtMC__muR_ct__muF_dn");
  h_wgtMC__muR_up__muF_ct = ctx.declare_event_output<float>("wgtMC__muR_up__muF_ct");
  h_wgtMC__muR_up__muF_up = ctx.declare_event_output<float>("wgtMC__muR_up__muF_up");
  h_wgtMC__muR_dn__muF_ct = ctx.declare_event_output<float>("wgtMC__muR_dn__muF_ct");
  h_wgtMC__muR_dn__muF_dn = ctx.declare_event_output<float>("wgtMC__muR_dn__muF_dn");
  //

  ////
}

bool TTbarLJAnalysisLiteModule::process(uhh2::Event& event){

  if(!event.isRealData){

    ttgenprod->process(event);
  }

  //// Data/MC scale factors

  float w_GEN(1.);
  float w_pileupSF_ct(1.), w_pileupSF_up(1.), w_pileupSF_dn(1.);
  float w_btagSF_ct(1.), w_btagSF_upL(1.), w_btagSF_dnL(1.), w_btagSF_upB(1.), w_btagSF_dnB(1.);
  float w_ttagSF_ct(1.), w_ttagSF_upL(1.), w_ttagSF_dnL(1.), w_ttagSF_upT(1.), w_ttagSF_dnT(1.);
  float w_muR_ct__muF_up(1.), w_muR_ct__muF_dn(1.), w_muR_up__muF_ct(1.), w_muR_up__muF_up(1.), w_muR_dn__muF_ct(1.), w_muR_dn__muF_dn(1.);

  if(!event.isRealData){

    w_GEN = event.weight;

    // pileup
    w_pileupSF_ct = pileupSF_ct->weight(event);
    w_pileupSF_up = pileupSF_up->weight(event);
    w_pileupSF_dn = pileupSF_dn->weight(event);
    //

    // b-tagging
    w_btagSF_ct  = btagSF_ct ->weight(event);

    w_btagSF_upL = btagSF_upL->weight(event);
    w_btagSF_dnL = btagSF_dnL->weight(event);

    w_btagSF_upB = btagSF_upB->weight(event);
    w_btagSF_dnB = btagSF_dnB->weight(event);
    //

    // t-tagging
    w_ttagSF_ct  = ttagSF_ct ->weight(event);

    w_ttagSF_upL = ttagSF_upL->weight(event);
    w_ttagSF_dnL = ttagSF_dnL->weight(event);

    w_ttagSF_upT = ttagSF_upT->weight(event);
    w_ttagSF_dnT = ttagSF_dnT->weight(event);
    //

    // Renormalization/Factorization scales
    if(event.genInfo){

      if(event.genInfo->systweights().size() >= 9){

        if(event.genInfo->originalXWGTUP()){

          w_muR_ct__muF_up = (event.genInfo->systweights().at(1) / event.genInfo->originalXWGTUP());
          w_muR_ct__muF_dn = (event.genInfo->systweights().at(2) / event.genInfo->originalXWGTUP());
          w_muR_up__muF_ct = (event.genInfo->systweights().at(3) / event.genInfo->originalXWGTUP());
          w_muR_up__muF_up = (event.genInfo->systweights().at(4) / event.genInfo->originalXWGTUP());
          w_muR_dn__muF_ct = (event.genInfo->systweights().at(6) / event.genInfo->originalXWGTUP());
          w_muR_dn__muF_dn = (event.genInfo->systweights().at(8) / event.genInfo->originalXWGTUP());
        }
      }
    }

    // central weight (histograms)
    event.weight  = w_GEN;
    event.weight *= w_pileupSF_ct;
    event.weight *= w_btagSF_ct;
    event.weight *= w_ttagSF_ct;
    //
  }
  //

  ////

  //// LEPTON SELECTION

  // OF lepton veto
  bool pass_lepV(0);
  if     (channel_ == muon) pass_lepV = ((event.electrons->size() == 0));
  else if(channel_ == elec) pass_lepV = ((event.muons    ->size() == 0));
  if(!pass_lepV) return false;
  //

  int lepN(-1);
  if     (channel_ == muon) lepN = int(event.muons    ->size());
  else if(channel_ == elec) lepN = int(event.electrons->size());
  if(!(lepN >= 1)) return false;

  ////

  //// JET selection

  /* 2nd AK4 jet selection */
  const bool pass_jet2 = jet2_sel->passes(event);
  if(!pass_jet2) return false;
  if(lepN == 1) HFolder("jet2")->fill(event);

  /* 1st AK4 jet selection */
  const bool pass_jet1 = jet1_sel->passes(event);
  if(!pass_jet1) return false;
  if(lepN == 1) HFolder("jet1")->fill(event);

  ////

  //// HLT selection
  const bool pass_trigger = trigger_sel->passes(event);
  if(!pass_trigger) return false;
  if(lepN == 1) HFolder("trigger")->fill(event);
  ////

  //// MET selection
  const bool pass_met = met_sel->passes(event);
  if(!pass_met) return false;
  if(lepN == 1) HFolder("met")->fill(event);
  ////

  //// HT_lep selection
  const bool pass_htlep = htlep_sel->passes(event);
  if(!pass_htlep) return false;
  if(lepN == 1) HFolder("htlep")->fill(event);
  ////

  //// TRIANGULAR-CUTS selection
  const bool pass_triangc = triangc_sel->passes(event);
  if(!pass_triangc) return false;
  if(lepN == 1) HFolder("triangc")->fill(event);
  ////

  //// TTBAR KIN RECO

  /* TOPTAG-EVENT boolean */
  const bool pass_ttagevt = ttagevt_sel->passes(event) && use_ttagging_;

  const std::string ttag_posx = (pass_ttagevt ? "t1" : "t0");
  /************************/

  reco_primlep->process(event);

  if(!pass_ttagevt){ ttbar_reco__ttag0->process(event); ttbar_chi2__ttag0->process(event); }
  else             { ttbar_reco__ttag1->process(event); ttbar_chi2__ttag1->process(event); }

  std::vector<ReconstructionHypothesis>& ttbar_hyps = event.get(h_ttbar_hyps);

  /* DATA blinding */
  if(blind_DATA_){

    if(lepN == 1){

      const ReconstructionHypothesis* rec_ttbar = get_best_hypothesis(ttbar_hyps, "Chi2");

      const float rec_ttbar_M((rec_ttbar->top_v4()+rec_ttbar->antitop_v4()).M());

      if(!( rec_ttbar_M < 2000. )) return false;
    }
  }
  /*****************/

  ////

  //// LEPTONIC-TOP pt selection
  const bool pass_topleppt = topleppt_sel->passes(event);
  if(!pass_topleppt) return false;
  if(lepN == 1) HFolder("topleppt")->fill(event);
  ////

  /* btagN counters */
  int jetbtagN(0), subjbtagN(0);

  const float jetbtagWP(.890), subjbtagWP(.890);

  if(!pass_ttagevt){

    for(const auto& j : *event.jets)
      if(j.btag_combinedSecondaryVertex() >  jetbtagWP)  ++jetbtagN;
  }
  else {

    const ReconstructionHypothesis* ttbar_hyp = get_best_hypothesis(ttbar_hyps, "Chi2");

    const TopJet* thad = ttbar_hyp->tophad_topjet_ptr();

    for(const auto& j : *event.jets){

      if(!(uhh2::deltaR(*thad, j) > ttag_minDR_jet_)) continue;
      if(j.btag_combinedSecondaryVertex() >  jetbtagWP)  ++jetbtagN;
    }

    for(const auto& j : thad->subjets())
      if(j.btag_combinedSecondaryVertex() > subjbtagWP) ++subjbtagN;
  }

  const int btagN = jetbtagN + subjbtagN;

  const std::string btag_posx = (btagN >= 2 ? "b2" : (btagN >= 1 ? "b1" : "b0"));
  /******************/

  //// FINAL KINEMATIC selection
  if(lepN == 1){

    HFolder("kine")       ->fill(event);
    HFolder("kine__ttbar")->fill(event);

    HFolder("kine__"+ttag_posx+btag_posx)          ->fill(event);
    HFolder("kine__"+ttag_posx+btag_posx+"__ttbar")->fill(event);
  }
  ////

//!!  //// inverted CHI2(top-lep) selection
//!!  const bool pass_antichi2tlep = antichi2tlep_sel->passes(event);
//!!  if(pass_antichi2tlep){
//!!
//!!    if(lepN == 1){
//!!
//!!      HFolder("antichi2tlep")       ->fill(event);
//!!      HFolder("antichi2tlep__BTAG") ->fill(event);
//!!      HFolder("antichi2tlep__TTAG") ->fill(event);
//!!      HFolder("antichi2tlep__ttbar")->fill(event);
//!!
//!!      HFolder("antichi2tlep__"+ttag_posx+btag_posx)          ->fill(event);
//!!      HFolder("antichi2tlep__"+ttag_posx+btag_posx+"__BTAG") ->fill(event);
//!!      HFolder("antichi2tlep__"+ttag_posx+btag_posx+"__TTAG") ->fill(event);
//!!      HFolder("antichi2tlep__"+ttag_posx+btag_posx+"__ttbar")->fill(event);
//!!    }
//!!  }
//!!  ////

  //// CHI2 selection
  const bool pass_chi2 = chi2_sel->passes(event);

  const std::string chi2_posx = pass_chi2 ? "chi2" : "antichi2";

  if(lepN == 1){

    HFolder(chi2_posx)          ->fill(event);
    HFolder(chi2_posx+"__ttbar")->fill(event);

    HFolder(chi2_posx+"__"+ttag_posx+btag_posx)          ->fill(event);
    HFolder(chi2_posx+"__"+ttag_posx+btag_posx+"__ttbar")->fill(event);
  }
  else if(lepN == 2){

    bool l2_ossf(false);
    if     (channel_ == muon) l2_ossf = ((event.muons    ->at(0).charge() * event.muons    ->at(1).charge()) == -1);
    else if(channel_ == elec) l2_ossf = ((event.electrons->at(0).charge() * event.electrons->at(1).charge()) == -1);

    if(l2_ossf && chi2_posx == "chi2"){

      HFolder(chi2_posx+"_L2")          ->fill(event);
      HFolder(chi2_posx+"_L2"+"__dilep")->fill(event);
      HFolder(chi2_posx+"_L2"+"__ttbar")->fill(event);

      HFolder(chi2_posx+"_L2"+"__"+ttag_posx+btag_posx)          ->fill(event);
      HFolder(chi2_posx+"_L2"+"__"+ttag_posx+btag_posx+"__dilep")->fill(event);
      HFolder(chi2_posx+"_L2"+"__"+ttag_posx+btag_posx+"__ttbar")->fill(event);
    }
  }
  ////

  //// VARS

  // event
  event.set(h_run       , event.run);
  event.set(h_lumi_block, event.luminosityBlock);
  event.set(h_event     , event.event);

  event.set(h_pvN  , event.pvs->size());

  event.set(h_muoN , event.muons    ->size());
  event.set(h_eleN , event.electrons->size());

  int ttagN(0);
  for(const auto& tj : *event.topjets) if(ttag_ID_(tj, event)) ++ttagN;

  event.set(h_btagN, btagN);
  event.set(h_ttagN, ttagN);

  event.set(h_ttagevt, pass_ttagevt);
  //

  // lepton
  const Particle* lep1 = leading_lepton(event);
  const Particle* lep2(0);
  if(lepN == 2){

    if     (channel_ == muon) lep2 = &((lep1 == &event.muons    ->at(0)) ? event.muons    ->at(1) : event.muons    ->at(0));
    else if(channel_ == elec) lep2 = &((lep1 == &event.electrons->at(0)) ? event.electrons->at(1) : event.electrons->at(0));
  }

  TLorentzVector lep1__p4(0.,0.,0.,0.), lep2__p4(0.,0.,0.,0.);
  int   lep1__pdgID(0), lep1__charge(0), lep2__pdgID(0), lep2__charge(0);
  float lep1__minDR_jet(-1.), lep1__pTrel_jet(-1.), lep2__minDR_jet(-1.), lep2__pTrel_jet(-1.);
  if(channel_ == muon){

    lep1__pdgID     = lep1->charge() * -13;
    lep1__charge    = lep1->charge();
    lep1__minDR_jet = ((Muon*) lep1)->get_tag(Muon::twodcut_dRmin);
    lep1__pTrel_jet = ((Muon*) lep1)->get_tag(Muon::twodcut_pTrel);
    lep1__p4        = TLorentzVector(lep1->v4().Px(), lep1->v4().Py(), lep1->v4().Pz(), lep1->v4().E());

    if(lep2){

      lep2__pdgID     = lep2->charge() * -13;
      lep2__charge    = lep2->charge();
      lep2__minDR_jet = ((Muon*) lep2)->get_tag(Muon::twodcut_dRmin);
      lep2__pTrel_jet = ((Muon*) lep2)->get_tag(Muon::twodcut_pTrel);
      lep2__p4        = TLorentzVector(lep2->v4().Px(), lep2->v4().Py(), lep2->v4().Pz(), lep2->v4().E());
    }
  }
  else if(channel_ == elec){

    lep1__pdgID     = lep1->charge() * -11;
    lep1__charge    = lep1->charge();
    lep1__minDR_jet = ((Electron*) lep1)->get_tag(Electron::twodcut_dRmin);
    lep1__pTrel_jet = ((Electron*) lep1)->get_tag(Electron::twodcut_pTrel);
    lep1__p4        = TLorentzVector(lep1->v4().Px(), lep1->v4().Py(), lep1->v4().Pz(), lep1->v4().E());

    if(lep2){

      lep2__pdgID     = lep2->charge() * -11;
      lep2__charge    = lep2->charge();
      lep2__minDR_jet = ((Electron*) lep2)->get_tag(Electron::twodcut_dRmin);
      lep2__pTrel_jet = ((Electron*) lep2)->get_tag(Electron::twodcut_pTrel);
      lep2__p4        = TLorentzVector(lep2->v4().Px(), lep2->v4().Py(), lep2->v4().Pz(), lep2->v4().E());
    }
  }

  event.set(h_lep1           , lep1__p4);
  event.set(h_lep1__pdgID    , lep1__pdgID);
  event.set(h_lep1__charge   , lep1__charge);
  event.set(h_lep1__minDR_jet, lep1__minDR_jet);
  event.set(h_lep1__pTrel_jet, lep1__pTrel_jet);

  event.set(h_lep2           , lep2__p4);
  event.set(h_lep2__pdgID    , lep2__pdgID);
  event.set(h_lep2__charge   , lep2__charge);
  event.set(h_lep2__minDR_jet, lep2__minDR_jet);
  event.set(h_lep2__pTrel_jet, lep2__pTrel_jet);
  //

  // MET
  event.set(h_MET, TLorentzVector(event.met->v4().Px(), event.met->v4().Py(), event.met->v4().Pz(), event.met->v4().P()));
  //

  // ttbar reco hyp
  const ReconstructionHypothesis* rec_ttbar = get_best_hypothesis(ttbar_hyps, "Chi2");
  if(!rec_ttbar) throw std::runtime_error("TTbarLJAnalysisLiteModule::process -- logic error: ttbar reconstruction hypothesis (\"get_best_hypothesis\", discr=Chi2) not found");

  const float MET__pz = rec_ttbar->neutrino_v4().Pz();
  event.set(h_MET__pz, MET__pz);

  const float rec_chi2 = rec_ttbar->discriminator("Chi2");
  event.set(h_rec_chi2, rec_chi2);

  const LorentzVector tlep(rec_ttbar->toplep_v4());
  const LorentzVector thad(rec_ttbar->tophad_v4());

  event.set(h_rec_tlep, TLorentzVector(tlep.Px(), tlep.Py(), tlep.Pz(), tlep.E()));
  event.set(h_rec_thad, TLorentzVector(thad.Px(), thad.Py(), thad.Pz(), thad.E()));
  //

  // weight
  event.set(h_wgtMC__GEN        , w_GEN);

  event.set(h_wgtMC__pileupSF_ct, w_pileupSF_ct);
  event.set(h_wgtMC__pileupSF_up, w_pileupSF_up);
  event.set(h_wgtMC__pileupSF_dn, w_pileupSF_dn);

  event.set(h_wgtMC__btagSF_ct  , w_btagSF_ct );
  event.set(h_wgtMC__btagSF_upL , w_btagSF_upL);
  event.set(h_wgtMC__btagSF_dnL , w_btagSF_dnL);
  event.set(h_wgtMC__btagSF_upB , w_btagSF_upB);
  event.set(h_wgtMC__btagSF_dnB , w_btagSF_dnB);

  event.set(h_wgtMC__ttagSF_ct  , w_ttagSF_ct );
  event.set(h_wgtMC__ttagSF_upL , w_ttagSF_upL);
  event.set(h_wgtMC__ttagSF_dnL , w_ttagSF_dnL);
  event.set(h_wgtMC__ttagSF_upT , w_ttagSF_upT);
  event.set(h_wgtMC__ttagSF_dnT , w_ttagSF_dnT);

  event.set(h_wgtMC__muR_ct__muF_up, w_muR_ct__muF_up);
  event.set(h_wgtMC__muR_ct__muF_dn, w_muR_ct__muF_dn);
  event.set(h_wgtMC__muR_up__muF_ct, w_muR_up__muF_ct);
  event.set(h_wgtMC__muR_up__muF_up, w_muR_up__muF_up);
  event.set(h_wgtMC__muR_dn__muF_ct, w_muR_dn__muF_ct);
  event.set(h_wgtMC__muR_dn__muF_dn, w_muR_dn__muF_dn);
  //

  ////

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(TTbarLJAnalysisLiteModule)
