#include "fa250dec.c"

int decodedData[3];

void display()
{
	Decoder::THaCodaFile f("eel122_23.dat.0");

	TFile *file = new TFile("output.root", "RECREATE");

	int adc1, adc2, chan;

	UInt_t * buf;
	int size = f.getBuffSize();
	
	buf = f.getEvBuffer();
	f.codaRead();
	buf = f.getEvBuffer();
	f.codaRead();
	buf = f.getEvBuffer();
	f.codaRead();
    
    // Initialize the tree (github edit)
    TTree *tree = new TTree("tree", "Decoded Data Tree");
    tree->Branch("adc1", &adc1, "adc1/I");
    tree->Branch("adc2", &adc2, "adc2/I");
	tree->Branch("chan", &chan, "chan/I");

	for (int i = 0 ; i < size; i++)
	{
		fa250DataDecode(buf[i]);
		//std::tie(adc1, adc2, chan) = std::make_tuple(decodedData[0], decodedData[1], decodedData[2]);
		adc1 = decodedData[0];
        adc2 = decodedData[1];
		chan = decodedData[2];
		tree->Fill();
    	if (i%10==0) cout<<endl;
		
	}
	 // Get the number of entries in the tree
    Long64_t nEntries = tree->GetEntries();

	// Write the tree to the file and close it
    tree->Write();
    file->Close();
}
// copy into array, then into tree
