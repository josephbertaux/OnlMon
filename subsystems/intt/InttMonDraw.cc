#include "InttMonDraw.h"

int InttMonDraw ::Draw(const std::string& what = "ALL")
{
  int opt = 0;  //Unnormalized HitMap
  if (what == "ALL" || what == WHAT[opt])
  {
    OnlMonClient* omc = OnlMonClient::instance();

    int layer;
    int hist;
    int bin;

    TH2D* hitmap;

    for (layer = 0; layer < LAYER; layer++)
    {
      hist = opt * LAYER + layer;
      if (!InttHist[hist])
      {
        InttHist[hist] = new TH2D(
            Form("INTT_%s_Layer%d", WHAT[opt].c_str(), layer + LAYER_OFFSET),
            Form("INTT_%s_Layer%d", WHAT[opt].c_str(), layer + LAYER_OFFSET),
            2 * LADDER[hist],
            0.0,
            LADDER[hist],
            CHIP,
            0.0,
            CHIP);
        //some stuff to configure this hist
      }

      hitmap = (TH2D*) omc->getHisto(Form("Layer%d_HitMap", layer));
      for (bin = 1; bin <= hitmap->GetNcells(); bin++)
      {
        InttHist[hist]->SetBinContent(bin, hitmap->GetBinContent(bin));
      }
    }
  }

  return 0;
}

void InttMonDraw ::DrawIntt(int opt)
{
  if (!(0 <= opt && opt < OPT)) return;

  if (!InttCanvas[opt])
  {
    InttCanvas[opt] = new TCanvas(
        Form("INTT_%s_Canvas", WHAT[opt].c_str()),
        Form("INTT_%s_Canvas", WHAT[opt].c_str()),
        20 * opt,  //-1, //no menu bar
        20 * opt,
        CNVS_WIDTH,
        CNVS_HEIGHT);

    gSystem->ProcessEvents();
  }

  int base_pad;
  int hist;
  int pad;

  for (int layer = 0; layer < LAYER; layer++)
  {
    base_pad = opt * LAYER * MAX_PADS + layer * MAX_PADS;
    hist = opt * LAYER + layer;
    pad = 0;

    if (!InttPad[base_pad])  //base of the layer drawing scheme
    {
      InttCanvas[opt]->cd();

      InttPad[base_pad] = new TPad(
          Form("INTT_Pad_%d", base_pad),
          Form("INTT_Pad_%d", base_pad),
          0.0,
          (LAYER - layer - 1.0) / LAYER,
          1.0,
          (LAYER - layer + 0.0) / LAYER);
      DrawPad(InttPad[base_pad]);
    }

    pad++;
    if (!InttPad[base_pad + pad])  //hist pad
    {
      InttPad[base_pad]->cd();
      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          KEY_FRAC,
          LABEL_FRAC,
          1.0,
          1.0 - TITLE_FRAC);
      DrawPad(InttPad[base_pad + pad]);
    }

    InttPad[base_pad + pad]->cd();
    if (InttHist[hist]) InttHist[hist]->Draw("COLZ");
    InttPad[base_pad + pad]->Update();

    pad++;
    if (!InttPad[base_pad + pad])  //grid pad
    {
      InttPad[base_pad]->cd();

      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          KEY_FRAC,
          LABEL_FRAC,
          1.0,
          1.0 - TITLE_FRAC);
      DrawPad(InttPad[base_pad + pad]);

      DrawGrid(InttPad[base_pad + pad], layer + LAYER_OFFSET);
    }
    InttPad[base_pad + pad]->Update();

    pad++;
    if (!InttPad[base_pad + pad])  //key pad
    {
      InttPad[base_pad]->cd();

      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          0.0,
          LABEL_FRAC,
          KEY_FRAC,
          1.0 - TITLE_FRAC);
      DrawPad(InttPad[base_pad + pad]);

      DrawKey(InttPad[base_pad + pad], layer + LAYER_OFFSET);
    }
    InttPad[base_pad + pad]->Update();

    pad++;
    if (!InttPad[base_pad + pad])  //lader labels
    {
      InttPad[base_pad]->cd();

      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          KEY_FRAC,
          0.0,
          1.0,
          LABEL_FRAC);
      DrawPad(InttPad[base_pad + pad]);

      DrawLabels(InttPad[base_pad + pad], layer + LAYER_OFFSET);
    }
    InttPad[base_pad + pad]->Update();

    pad++;
    if (!InttPad[base_pad + pad])  //Title
    {
      InttPad[base_pad]->cd();

      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          KEY_FRAC,
          1.0 - TITLE_FRAC,
          1.0,
          1.0);
      DrawPad(InttPad[base_pad + pad]);

      DrawTitle(InttPad[base_pad + pad], layer + LAYER_OFFSET);
    }
    InttPad[base_pad + pad]->Update();

    pad++;
    if (!InttPad[base_pad + pad])  //exec pad
    {
      InttPad[base_pad]->cd();
      InttPad[base_pad + pad] = new TPad(
          Form("INTT_Pad_%d", base_pad + pad),
          Form("INTT_Pad_%d", base_pad + pad),
          KEY_FRAC + L_MARGIN * (1.0 - KEY_FRAC),
          LABEL_FRAC + B_MARGIN * (1.0 - TITLE_FRAC - LABEL_FRAC),
          1.0 - R_MARGIN * (1.0 - KEY_FRAC),
          1.0 - TITLE_FRAC - T_MARGIN * (1.0 - TITLE_FRAC - LABEL_FRAC));
      DrawPad(InttPad[base_pad + pad]);
      SetExec(InttPad[base_pad + pad], opt, layer + LAYER_OFFSET);
    }

    InttCanvas[opt]->Update();
    InttCanvas[opt]->Show();
    InttCanvas[opt]->SetEditable(false);
  }
}

void InttMonDraw ::DrawPad(TPad* pad)
{
  pad->SetFillStyle(4000);  //transparent
  pad->Range(0.0, 0.0, 1.0, 1.0);
  pad->SetTopMargin(T_MARGIN);
  pad->SetBottomMargin(B_MARGIN);
  pad->SetLeftMargin(L_MARGIN);
  pad->SetRightMargin(R_MARGIN);
  pad->Draw();
}

void InttMonDraw ::DrawKey(TPad* pad, int layer)
{
  layer -= LAYER_OFFSET;

  if (!pad) return;

  pad->cd();

  //want to draw it to scale with the histogram
  //the hist pad is drawn on (1.0 - KEY_FRAC) of the layer pad
  //the hist pad itself has margins L_MARGIN, R_MARGIN of the hist pad on either side
  //this is further divided into LADDER[layer] sections
  //thus a hist ladder is (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] of the layer pad wide

  //this should be as wide
  //the key is drawn on KEY_FRAC of the layer pad
  //so KEY_FRAC * width = (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer]

  //there are at least 12 ladders, these lines will never run
  //in principle, it could be a problem so I am being pedantic

  //shift rightward slightly by half the distance to the edge of pad
  //implemented several times over below
  //inside if statements; redundant, but extra variables will not be allocated each time this runs

  //gridlines
  if (!key_vline[layer])
  {
    double x;

    double xmin = 0.5 - (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;
    double xmax = 0.5 + (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;

    if (xmin < 0.0) xmin = 0.1;
    if (xmax > 1.0) xmax = 0.9;

    xmax += xmin / 2.0;
    xmin += xmin / 2.0;

    key_vline[layer] = new TLine*[3];
    for (int v = 0; v < 3; v++)
    {
      x = xmin + v * (xmax - xmin) / 2;

      key_vline[layer][v] = new TLine(x, B_MARGIN, x, 1.0 - T_MARGIN);
      key_vline[layer][v]->SetLineStyle(2 * (v % 2) + 1);
      key_vline[layer][v]->SetLineWidth(2 - (v % 2));
    }
  }
  for (int v = 0; v < 3; v++)
  {
    key_vline[layer][v]->Draw();
  }

  if (!key_hline[layer])
  {
    double y;

    double xmin = 0.5 - (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;
    double xmax = 0.5 + (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;

    if (xmin < 0.0) xmin = 0.1;
    if (xmax > 1.0) xmax = 0.9;

    xmax += xmin / 2.0;
    xmin += xmin / 2.0;

    key_hline[layer] = new TLine*[CHIP + 1];
    for (int h = 0; h < CHIP + 1; h++)
    {
      y = B_MARGIN + h * (1.0 - T_MARGIN - B_MARGIN) / CHIP;

      key_hline[layer][h] = new TLine(xmin, y, xmax, y);
      key_hline[layer][h]->SetLineStyle(3);
      key_hline[layer][h]->SetLineWidth(1);
      if (h % (CHIP / 2) == 0)
      {
        key_hline[layer][h]->SetLineStyle(1);
        key_hline[layer][h]->SetLineWidth(2);
      }
    }
  }
  for (int h = 0; h < CHIP + 1; h++)
  {
    key_hline[layer][h]->Draw();
  }

  //text
  if (!key_nlabel[layer])
  {
    double x;
    double y;

    double xmin = 0.5 - (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;
    double xmax = 0.5 + (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;

    if (xmin < 0.0) xmin = 0.1;
    if (xmax > 1.0) xmax = 0.9;

    xmax += xmin / 2.0;
    xmin += xmin / 2.0;

    key_nlabel[layer] = new TText*[CHIP + 1];
    for (int chip = 0; chip < CHIP; chip++)
    {
      x = ((3 - 2 * (chip / (CHIP / 2))) * xmax + (1 + 2 * (chip / (CHIP / 2))) * xmin) / 4.0;
      y = (1.0 - T_MARGIN + B_MARGIN) / 2.0 + (CHIP / 2 - chip % (CHIP / 2) - 0.5) * (0.5 - T_MARGIN / 2.0 - B_MARGIN / 2.0) / (CHIP / 2);

      key_nlabel[layer][chip] = new TText(x, y, Form("U%d", chip + CHIP_OFFSET));
      key_nlabel[layer][chip]->SetTextAlign(22);
      key_nlabel[layer][chip]->SetTextSize(KEY_TEXT_SIZE2);
    }

    key_nlabel[layer][CHIP] = new TText(
        xmin / 2.0,
        0.75,
        Form("North"));
    key_nlabel[layer][CHIP]->SetTextAlign(22);
    key_nlabel[layer][CHIP]->SetTextSize(KEY_TEXT_SIZE1);
    key_nlabel[layer][CHIP]->SetTextAngle(90);
  }
  for (int chip = 0; chip < CHIP + 1; chip++)
  {
    key_nlabel[layer][chip]->Draw();
  }

  if (!key_slabel[layer])
  {
    double x;
    double y;

    double xmin = 0.5 - (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;
    double xmax = 0.5 + (1.0 - KEY_FRAC) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer] / KEY_FRAC / 2;

    if (xmin < 0.0) xmin = 0.1;
    if (xmax > 1.0) xmax = 0.9;

    xmax += xmin / 2.0;
    xmin += xmin / 2.0;

    key_slabel[layer] = new TText*[CHIP + 1];
    for (int chip = 0; chip < CHIP; chip++)
    {
      x = ((1 + 2 * (chip / (CHIP / 2))) * xmax + (3 - 2 * (chip / (CHIP / 2))) * xmin) / 4.0;
      y = B_MARGIN + (chip % (CHIP / 2) + 0.5) * (0.5 - T_MARGIN / 2.0 - B_MARGIN / 2.0) / (CHIP / 2);

      key_slabel[layer][chip] = new TText(x, y, Form("U%d", chip + CHIP_OFFSET));
      key_slabel[layer][chip]->SetTextAlign(22);
      key_slabel[layer][chip]->SetTextSize(KEY_TEXT_SIZE2);
    }

    key_slabel[layer][CHIP] = new TText(
        xmin / 2.0,
        0.25,
        Form("South"));
    key_slabel[layer][CHIP]->SetTextAlign(22);
    key_slabel[layer][CHIP]->SetTextSize(KEY_TEXT_SIZE1);
    key_slabel[layer][CHIP]->SetTextAngle(90);
  }

  for (int chip = 0; chip < CHIP + 1; chip++)
  {
    key_slabel[layer][chip]->Draw();
  }
}

void InttMonDraw ::DrawGrid(TPad* pad, int layer)
{
  layer -= LAYER_OFFSET;

  if (!pad) return;

  pad->cd();

  if (!grid_vline[layer])
  {
    double temp;

    grid_vline[layer] = new TLine*[2 * LADDER[layer] + 1];
    for (int v = 0; v < 2 * LADDER[layer] + 1; v++)
    {
      temp = L_MARGIN + (v / 2.0) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer];

      grid_vline[layer][v] = new TLine(temp, B_MARGIN, temp, 1.0 - T_MARGIN);
      grid_vline[layer][v]->SetLineStyle(2 * (v % 2) + 1);  //3 if odd, 1 if even
      grid_vline[layer][v]->SetLineWidth(2 - (v % 2));
    }
  }
  for (int v = 0; v < 2 * LADDER[layer] + 1; v++)
  {
    grid_vline[layer][v]->Draw();
  }

  if (!grid_hline[layer])
  {
    double temp;

    grid_hline[layer] = new TLine*[CHIP + 1];
    for (int h = 0; h < CHIP + 1; h++)
    {
      temp = B_MARGIN + h * (1.0 - T_MARGIN - B_MARGIN) / CHIP;

      grid_hline[layer][h] = new TLine(L_MARGIN, temp, 1.0 - R_MARGIN, temp);
      grid_hline[layer][h]->SetLineStyle(3);
      grid_hline[layer][h]->SetLineWidth(1);
      if (h % (CHIP / 2) == 0)
      {
        grid_hline[layer][h]->SetLineStyle(1);
        grid_hline[layer][h]->SetLineWidth(2);
      }
    }
  }
  for (int h = 0; h < CHIP + 1; h++)
  {
    grid_hline[layer][h]->Draw();
  }
}

void InttMonDraw ::DrawLabels(TPad* pad, int layer)
{
  layer -= LAYER_OFFSET;

  if (!pad) return;

  pad->cd();

  if (!label[layer])
  {
    double temp;

    label[layer] = new TText*[LADDER[layer] + 1];
    for (int l = 0; l < LADDER[layer]; l++)
    {
      temp = L_MARGIN + (l + 0.5) * (1.0 - L_MARGIN - R_MARGIN) / LADDER[layer];

      label[layer][l] = new TText(temp, 0.875, Form("%d", l));
      label[layer][l]->SetTextAlign(22);
      label[layer][l]->SetTextSize(LABEL_TEXT_SIZE2);
    }

    temp = (L_MARGIN + 1.0 - R_MARGIN) / 2.0;
    label[layer][LADDER[layer]] = new TText(temp, 0.5, Form("Ladder"));
    label[layer][LADDER[layer]]->SetTextAlign(22);
    label[layer][LADDER[layer]]->SetTextSize(LABEL_TEXT_SIZE1);
  }
  for (int l = 0; l < LADDER[layer] + 1; l++)
  {
    label[layer][l]->Draw();
  }
}

void InttMonDraw ::DrawTitle(TPad* pad, int layer)
{
  layer -= LAYER_OFFSET;

  if (!pad) return;

  pad->cd();

  if (!title[layer])
  {
    double temp = (L_MARGIN + 1.0 - R_MARGIN) / 2.0;
    title[layer] = new TText(temp, 0.5, Form("Layer %d", layer + LAYER_OFFSET));
    title[layer]->SetTextAlign(22);
    title[layer]->SetTextSize(TITLE_TEXT_SIZE);
  }
  title[layer]->Draw();
}

void InttMonDraw ::SetExec(TPad* pad, int opt, int layer)
{
  layer -= LAYER_OFFSET;

  if (!(0 <= opt && opt < OPT)) return;
  if (!(0 <= layer && layer < LAYER)) return;

  std::stringstream func_str;
  func_str << CMND_NAME[opt] << "(" << layer + LAYER_OFFSET << ")";
  pad->AddExec(EXEC_NAME[opt].c_str(), func_str.str().c_str());
}