// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Clemens Groepl $
// $Authors: Katharina Albers, Clemens Groepl, Chris Bielow, Mathias Walzer $
// --------------------------------------------------------------------------

#include <OpenMS/FORMAT/SequestOutfile.h>
#include <OpenMS/FORMAT/IdXMLFile.h>
#include <OpenMS/FORMAT/PepXMLFile.h>
#include <OpenMS/FORMAT/OMSSAXMLFile.h>
#include <OpenMS/FORMAT/MascotXMLFile.h>
#include <OpenMS/FORMAT/ProtXMLFile.h>
#include <OpenMS/FORMAT/FileHandler.h>
#include <OpenMS/FORMAT/FileTypes.h>
#include <OpenMS/FORMAT/MzIdentMLFile.h>

#include <OpenMS/APPLICATIONS/TOPPBase.h>

using namespace OpenMS;
using namespace std;

//-------------------------------------------------------------
// Doxygen docu
//-------------------------------------------------------------

/**
    @page TOPP_IDFileConverter IDFileConverter

    @brief Converts identification engine file formats.

<CENTER>
    <table>
        <tr>
            <td ALIGN = "center" BGCOLOR="#EBEBEB"> potential predecessor tools </td>
            <td VALIGN="middle" ROWSPAN=3> \f$ \longrightarrow \f$ IDFileConverter \f$ \longrightarrow \f$</td>
            <td ALIGN = "center" BGCOLOR="#EBEBEB"> potential successor tools </td>
        </tr>
        <tr>
            <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> TPP tools: PeptideProphet, ProteinProphet </td>
            <td VALIGN="middle" ALIGN = "center" ROWSPAN=2> TPP tools: ProteinProphet\n(for conversion from idXML to pepXML) </td>
        </tr>
        <tr>
            <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> Sequest protein identification engine </td>
        </tr>
    </table>
</CENTER>


Conversion from the TPP file formats pepXML and protXML to OpenMS' idXML is quite comprehensive, to the extent that the original data can be
represented in the simpler idXML format.

In contrast, support for converting from idXML to pepXML is limited. The purpose here is simply to create pepXML files containing the relevant
information for the use of ProteinProphet.

<B>Details on additional parameters:</B>

@p mz_file: \n
Some search engine output files (like pepXML, mascotXML, Sequest .out files) may not contain retention times, only scan numbers. To be able to look up the actual RT values, the raw file has to be provided using the parameter @p mz_file. (If the identification results should be used later to annotate feature maps or consensus maps, it is critical that they contain RT values. See also @ref TOPP_IDMapper.)

@p mz_name: \n
PepXML files can contain results from multiple experiments. However, the idXML format does not support this. The @p mz_name parameter (or @p mz_file, if given) thus serves to define what parts to extract from the pepXML.

@p scan_regex: \n
For Mascot results exported to XML, the scan numbers (used to look up retention times using @p mz_file) should be given in the "pep_scan_title" XML elements, but the format can vary. If the defaults fail to extract the scan numbers, a Perl-style regular expression can be given through the advanced parameter @p scan_regex, and will be used instead. The regular expression should contain a named group "SCAN" matching the scan number or "RT" matching the actual retention time. For example, if the format of the "pep_scan_title" elements is "scan=123", where 123 is the scan number, the expression "scan=(?<SCAN>\\d+)" can be used to extract the number. (However, the format in this example is actually covered by the defaults.)


Some information about the supported input types:
  @ref OpenMS::MzIdentMLFile "mzIdentML"
  @ref OpenMS::PepXMLFile "pepXML"
  @ref OpenMS::ProtXMLFile "protXML"
  @ref OpenMS::IdXMLFile "idXML"
  @ref OpenMS::MascotXMLFile "mascotXML"
  @ref OpenMS::OMSSAXMLFile "omssaXML"
  @ref OpenMS::SequestOutfile "Sequest .out directory"

    <B>The command line parameters of this tool are:</B>
    @verbinclude TOPP_IDFileConverter.cli
    <B>INI file documentation of this tool:</B>
    @htmlinclude TOPP_IDFileConverter.html

*/

// We do not want this class to show up in the docu:
/// @cond TOPPCLASSES


class TOPPIDFileConverter :
  public TOPPBase
{
public:
  TOPPIDFileConverter() :
    TOPPBase("IDFileConverter", "Converts identification engine file formats.", true)
  {
  }

protected:
  void
  registerOptionsAndFlags_()
  {
    registerInputFile_("in", "<path/file>", "", "Input file or directory containing the output of the search engine.\n"
                                                "Sequest: Directory containing the .out files\n"
                                                "pepXML: Single pepXML file.\n"
                                                "protXML: Single protXML file.\n"
                                                "mascotXML: Single Mascot XML file.\n"
                                                "omssaXML: Single OMSSA XML file.\n"
                                                "idXML: Single idXML file.\n", true);
    setValidFormats_("in", StringList::create("pepXML,protXML,mascotXML,omssaXML,idXML"));

    registerOutputFile_("out", "<file>", "", "Output file", true);
    String formats("idXML,mzid,pepXML,FASTA");
    setValidFormats_("out", StringList::create(formats));
    registerStringOption_("out_type", "<type>", "", "output file type -- default: determined from file extension or content\n", false);
    setValidStrings_("out_type", StringList::create(formats));

    addEmptyLine_();
    registerInputFile_("mz_file", "<file>", "", "[Sequest, pepXML, mascotXML only] Retention times will be looked up in this file", false);
    setValidFormats_("mz_file", StringList::create("mzML,mzXML,mzData"));
    addEmptyLine_();
    registerFlag_("ignore_proteins_per_peptide", "[Sequest only] Workaround to deal with .out files that contain e.g. \"+1\" in references column,\n"
                                                 "but do not list extra references in subsequent lines (try -debug 3 or 4)", true);
    registerStringOption_("mz_name", "<file>", "", "[pepXML only] Experiment filename/path (extension will be removed) to match in the pepXML file ('base_name' attribute). Only necessary if different from 'mz_file'.", false);
    registerFlag_("use_precursor_data", "[pepXML only] Use precursor RTs (and m/z values) from 'mz_file' for the generated peptide identifications, instead of the RTs of MS2 spectra.", false);
    registerStringOption_("scan_regex", "<expression>", "", "[mascotXML only] Regular expression used to extract the scan number or retention time. See documentation for details.", false, true);
  }

  ExitCodes
  main_(int, const char **)
  {
    //-------------------------------------------------------------
    // general variables and data
    //-------------------------------------------------------------
    FileHandler fh;
    vector<PeptideIdentification> peptide_identifications;
    vector<ProteinIdentification> protein_identifications;

    //-------------------------------------------------------------
    // reading input
    //-------------------------------------------------------------
    const String in = getStringOption_("in");

    if (File::isDirectory(in))
    {
      const String in_directory = File::absolutePath(in).ensureLastChar('/');
      const String mz_file = getStringOption_("mz_file");
      const bool ignore_proteins_per_peptide = getFlag_("ignore_proteins_per_peptide");

      UInt i = 0;
      FileHandler fh;
      FileTypes::Type type;
      MSExperiment<Peak1D> msexperiment;
      // Note: we had issues with leading zeroes, so let us represent scan numbers as Int (next line used to be map<String, Real> num_and_rt;)  However, now String::toInt() might throw.
      map<Int, Real> num_and_rt;
      vector<String> NativeID;

      // The mz-File (if given)
      if (!mz_file.empty())
      {
        type = fh.getTypeByFileName(mz_file);
        fh.loadExperiment(mz_file, msexperiment, type);

        for (MSExperiment<Peak1D>::Iterator spectra_it = msexperiment.begin(); spectra_it != msexperiment.end(); ++spectra_it)
        {
          String(spectra_it->getNativeID()).split('=', NativeID);
          try
          {
            num_and_rt[NativeID[1].toInt()] = spectra_it->getRT();
            // std::cout << "num_and_rt: " << NativeID[1] << " = " << NativeID[1].toInt() << " : " << num_and_rt[NativeID[1].toInt()] << std::endl; // CG debuggging 2009-07-01
          }
          catch (Exception::ConversionError & e)
          {
            writeLog_(String("Error: Cannot read scan number as integer. '") + e.getMessage());
          }
        }
      }

      // Get list of the actual Sequest .out-Files
      StringList in_files;
      if (!File::fileList(in_directory, String("*.out"), in_files))
      {
        writeLog_(String("Error: No .out files found in '") + in_directory + "'. Aborting!");
      }

      // Now get to work ...
      for (vector<String>::const_iterator in_files_it = in_files.begin(); in_files_it != in_files.end(); ++in_files_it)
      {
        vector<PeptideIdentification> peptide_ids_seq;
        ProteinIdentification protein_id_seq;
        vector<DoubleReal> pvalues_seq;
        vector<String> in_file_vec;

        SequestOutfile sequest_outfile;

        writeDebug_(String("Reading file ") + *in_files_it, 3);

        try
        {
          sequest_outfile.load((String) (in_directory + *in_files_it), peptide_ids_seq, protein_id_seq, 1.0, pvalues_seq, "Sequest", ignore_proteins_per_peptide);

          in_files_it->split('.', in_file_vec);

          for (Size j = 0; j < peptide_ids_seq.size(); ++j)
          {

            // We have to explicitly set the identifiers, because the normal set ones are composed of search engine name and date, which is the same for a bunch of sequest out-files.
            peptide_ids_seq[j].setIdentifier(*in_files_it + "_" + i);

            Int scan_number = 0;
            if (!mz_file.empty())
            {
              try
              {
                scan_number = in_file_vec.at(2).toInt();
                peptide_ids_seq[j].setMetaValue("RT", num_and_rt[scan_number]);
              }
              catch (Exception::ConversionError & e)
              {
                writeLog_(String("Error: Cannot read scan number as integer. '") + e.getMessage());
              }
              catch (std::exception & e)
              {
                writeLog_(String("Error: Cannot read scan number as integer. '") + e.what());
              }
              //DoubleReal real_mz = ( (DoubleReal)peptide_ids_seq[j].getMetaValue("MZ") - hydrogen_mass )/ (DoubleReal)peptide_ids_seq[j].getHits()[0].getCharge(); // ???? semantics of mz
              const DoubleReal real_mz = (DoubleReal) peptide_ids_seq[j].getMetaValue("MZ") / (DoubleReal) peptide_ids_seq[j].getHits()[0].getCharge();
              peptide_ids_seq[j].setMetaValue("MZ", real_mz);
            }

            writeDebug_(String("scan: ") + String(scan_number) + String("  RT: ") + String(peptide_ids_seq[j].getMetaValue("RT")) + "  MZ: " + String(peptide_ids_seq[j].getMetaValue("MZ")) + "  Ident: " + peptide_ids_seq[j].getIdentifier(), 4);

            peptide_identifications.push_back(peptide_ids_seq[j]);
          }

          protein_id_seq.setIdentifier(*in_files_it + "_" + i);
          protein_identifications.push_back(protein_id_seq);
          ++i;
        }
        catch (Exception::ParseError & pe)
        {
          writeLog_(pe.getMessage() + String("(file: ") + *in_files_it + ")");
          throw;
        }
        catch (...)
        {
          writeLog_(String("Error reading file: ") + *in_files_it);
          throw;
        }
      }

      writeDebug_("All files processed.", 3);
    } // ! directory
    else
    {
      FileTypes::Type in_type = fh.getType(in);

      if (in_type == FileTypes::PEPXML)
      {
        String exp_name = getStringOption_("mz_file");
        String orig_name =  getStringOption_("mz_name");
        bool use_precursor_data = getFlag_("use_precursor_data");

        if (exp_name.empty())
        {
          PepXMLFile().load(in, protein_identifications, 
                            peptide_identifications, orig_name);
        }
        else
        {
          MSExperiment<> exp;
          fh.loadExperiment(exp_name, exp);
          if (!orig_name.empty())
          {
            exp_name = orig_name;
          }
          PepXMLFile().load(in, protein_identifications, 
                            peptide_identifications, exp_name, exp, 
                            use_precursor_data);
        }
      }
      else if (in_type == FileTypes::IDXML)
      {
        IdXMLFile().load(in, protein_identifications, peptide_identifications);
      }
      else if (in_type == FileTypes::PROTXML)
      {
        protein_identifications.resize(1);
        peptide_identifications.resize(1);
        ProtXMLFile().load(in, protein_identifications[0], 
                           peptide_identifications[0]);
      }
      else if (in_type == FileTypes::OMSSAXML)
      {
        protein_identifications.resize(1);
        OMSSAXMLFile().load(in, protein_identifications[0], 
                            peptide_identifications, true);
      }
      else if (in_type == FileTypes::MASCOTXML)
      {
        String scan_regex = getStringOption_("scan_regex");
        String exp_name = getStringOption_("mz_file");
        MascotXMLFile::RTMapping rt_mapping;
        if (!exp_name.empty())
        {
          PeakMap exp;
          // load only MS2 spectra:
          fh.getOptions().addMSLevel(2);
          fh.loadExperiment(exp_name, exp, FileTypes::MZML, log_type_);
          MascotXMLFile::generateRTMapping(exp.begin(), exp.end(), rt_mapping);
        }
        protein_identifications.resize(1);
        MascotXMLFile().load(in, protein_identifications[0],
                             peptide_identifications, rt_mapping, scan_regex);
      }
      else
      {
        writeLog_("Unknown input file type given. Aborting!");
        printUsage_();
        return ILLEGAL_PARAMETERS;
      }
    }

    //-------------------------------------------------------------
    // writing output
    //-------------------------------------------------------------
    const String out = getStringOption_("out");
    FileTypes::Type out_type = FileTypes::nameToType(getStringOption_("out_type"));
    if (out_type == FileTypes::UNKNOWN)
    {
      out_type = fh.getTypeByFileName(out);
    }
    if (out_type == FileTypes::UNKNOWN)
    {
      writeLog_("Error: Could not determine output file type!");
      return PARSE_ERROR;
    }

    if (out_type == FileTypes::PEPXML)
    {
      PepXMLFile().store(out, protein_identifications, peptide_identifications);
    }
    else if (out_type == FileTypes::IDXML)
    {
      IdXMLFile().store(out, protein_identifications, peptide_identifications);
    }
    else if (out_type == FileTypes::MZIDENTML)
    {
      MzIdentMLFile().store(out, protein_identifications, peptide_identifications);
    }
    else if (out_type == FileTypes::FASTA)
    {
      Size count = 0;
      std::ofstream fasta(out.c_str(), std::ios::out);
      for (Size i = 0; i < peptide_identifications.size(); ++i)
      {
        for (Size l = 0; l < peptide_identifications[i].getHits().size(); ++l)
        {
          const PeptideHit & hit = peptide_identifications[i].getHits()[l];
          fasta << ">" << hit.getSequence().toUnmodifiedString() << "|" << count++
                << "|" << hit.getSequence().toString() << endl;
          String seq = hit.getSequence().toUnmodifiedString();
          // FASTA files should have at most 60 characters of sequence info per line
          for (Size j = 0; j < seq.size(); j += 60)
          {
            Size k = min(j + 60, seq.size());
            fasta << std::string(seq[j], seq[k]) << endl;
          }
        }
      }
    }
    else
    {
      writeLog_("Unsupported output file type given. Aborting!");
      printUsage_();
      return ILLEGAL_PARAMETERS;
    }

    return EXECUTION_OK;
  }

};

int main(int argc, const char ** argv)
{
  TOPPIDFileConverter tool;
  return tool.main(argc, argv);
}

///@endcond
