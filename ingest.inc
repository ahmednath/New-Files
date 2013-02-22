<?php
function processall()
{
  try {
  	initupload();
  	global $user;
  	$filecounter = 0;

  	# create value array
  	$arr = array();
  	$arr['parent']['pid'] = getparentpid();
  	$arr['files'] = getuploadedfiles();
  	$arr['primaryfile']['name'] = getprimaryupload();
  	$arr['pid'] = getnextpid();
  	$arr['model'] = getcontentmodel();
  	$arr['label'] = $arr['primaryfile']['name'];
  	$arr['ownerId'] = $user->uid;
  	$arr['contact'] = $user->name;
  	$arr['timestamp'] = date("Y-m-d")."T".date("H:i:s")."Z";
  	$arr['meta'] = $_SESSION['meta'];
  	$arr['binary'] = filetobase64($GLOBALS['paths']['upload'].$arr['primaryfile']['name']);
  	$arr['primaryfile']['thumbnail'] = chunk_split(base64_encode(createthumbnail($GLOBALS['paths']['upload'].$arr['primaryfile']['name'])));
  	
  	$arr['secondaryfiles'] = array();
  	foreach ($arr['files'] as $file) {
  		if ($file != $arr['primaryfile']['name']) {
  			$arr['secondaryfiles'][$filecounter]['name'] = $file;
  			$arr['secondaryfiles'][$filecounter]['id'] = 'OBJ'.($filecounter+2);
  			$arr['secondaryfiles'][$filecounter]['binary'] = filetobase64($GLOBALS['paths']['upload'].$file);
  			$filecounter += 1;
  		}
  	}
  	
  	#echo '<pre>'; print_r($arr['secondaryfiles']); echo '</pre>'; exit;
  	
  	#$output = buildxml($arr); header("Content-Type: text/xml"); print $output; exit(0);
  	
  	$output = buildxml($arr);
  	file_put_contents($GLOBALS['paths']['upload'].'istir_out.xml', $output);
  	$ret = ingestfile();
  	return $ret;
  }
  catch (exception $e) {
  	return $e->getMessage();
  }

}

function ingestfile()
{
  #$output = shell_exec('/istir_load.sh '.session_id());
  $output = shell_exec("/istir_load.sh ".session_id()." 2>&1");
  return $output;
}

function filetobase64($filepath)
{
  $data = file_get_contents($filepath);
  $ret = chunk_split(base64_encode($data));
  return $ret;
}

function createthumbnail($filepath)
{
  $ext = getfileextension($filepath);
  
  if ($ext == 'pdf' || $ext == 'png' || $ext == 'jpg' || $ext == 'gif' || $ext == 'jpeg') 
  {
  	if ($ext == 'pdf') { $filepath .= '[0]'; }
  	
  	$im = new imagick($filepath);
  	$im->setImageFormat("jpg");
  	$im->setImageCompressionQuality(75);
  	$im->thumbnailImage(100,120,true,false);
  	$out = $im;
  }
  else
  {
  	$im = new imagick($GLOBALS['paths']['images'].'Crystal_Clear_mimetype_txt.png');
  	$im->setImageFormat("jpg");
  	$im->setImageCompressionQuality(75);
  	$im->thumbnailImage(100,120,true,false);
  	$out = $im;
  }

  return $out;
}

function getnextpid()
{
  if (!isset($_SESSION['meta']['pid']))
  {
  	module_load_include('inc', 'fedora_repository', 'ConnectionHelper');
  	$connectionHelper = new ConnectionHelper();

  	try {
  		$soapClient = $connectionHelper->getSoapClient(variable_get('fedora_soap_manage_url', 'http://localhost:8080/fedora/wsdl?api=API-M'));
  		#$pidNameSpace = variable_get('fedora_repository_pid', 'vre:');
  		#$pidNameSpace = substr($pidNameSpace, 0, strpos($pidNameSpace, ":"));
  		$pidNameSpace = $GLOBALS['namespace'];
  		$params = array(
  			'numPIDs' => '',
  			'pidNamespace' => $pidNameSpace
  		);
  		$object = $soapClient->__soapCall('getNextPID', array($params));
  		$pid = implode(get_object_vars($object));
  		$pidNumber = strstr($pid, ":");
  		$_SESSION['meta']['pid'] = $pidNameSpace.$pidNumber;
  		return $_SESSION['meta']['pid'];
  	} catch (exception $e) {
  		drupal_set_message(t('Error getting Next PID: @e', array('@e' => check_plain($e->getMessage()))), 'error');
  		return FALSE;
  	}
  }
  else
  {
  	return $_SESSION['meta']['pid'];
  }
}

function getcontentmodel()
{
  $ext = getfileextension(getprimaryupload());
  switch ($ext)
  {
  	case 'png': $ret = 'islandora:sp_basic_image'; break;
  	case 'jpg': $ret = 'islandora:sp_basic_image'; break;
  	case 'jpeg': $ret = 'islandora:sp_basic_image'; break;
  	case 'gif': $ret = 'islandora:sp_basic_image'; break;
  	case 'pdf': $ret = 'islandora:sp_strict_pdf'; break;
  	default: $ret = ''; break;
  }
  return $ret;
}

function getobjecttype()
{
  if (isset($_SESSION['meta']['object_type']))
  {
  	switch ($_SESSION['meta']['object_type'])
  	{
  		case 'technicalreport': return 'Technical Report'; break;
  		case 'presentation': return 'Presentation'; break;
  		case 'researcharticle': return 'Research Article/Paper'; break;
  		case 'technologyreport': return 'Technology/Engineering Report'; break;
  		case 'thesis': return 'Thesis'; break;
  		case 'map': return 'Map'; break;
  		default: return ''; break;
  	}
  }
  else
  {
  	return '';
  }
}

function getlabel()
{
  $out = '';

  if ($_SESSION['meta']['ara']['title']['value'] != '') {
  	$out = fval($_SESSION['meta']['ara']['title']['value']);
  }

  if ($_SESSION['meta']['eng']['title']['value'] != '') {
  	$out = fval($_SESSION['meta']['eng']['title']['value']);
  }

  return $out;
}

function getparentpid()
{
  if (isset($_SESSION['meta']['subjectcollection']))
  {
  	return (($_SESSION['meta']['subjectcollection'] == 'agriculture') ? 'istir:agriculture' : 'istir:environment');
  }
  else
  {
  	return 'islandora:root';
  }
}

function getmime($filename)
{
  $ext = getfileextension($filename);
  $mime = getmimetypebyextension($ext);
  return $mime;
}

function fval($in) {
  $out = trim($in);
  $out = htmlspecialchars($out);
  
  return $out;
}

function buildxml($in)
{

  $nl = "\r\n";

  $xml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= $nl.'<foxml:digitalObject PID="'.$in['pid'].'" VERSION="1.1" xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="info:fedora/fedora-system:def/foxml# http://www.fedora.info/definitions/1/0/foxml1-1.xsd">';
  $xml .= $nl.'	<foxml:objectProperties>';
  $xml .= $nl.'		<foxml:property NAME="info:fedora/fedora-system:def/model#state" VALUE="Inactive"/>';
  $xml .= $nl.'		<foxml:property NAME="info:fedora/fedora-system:def/model#label" VALUE="'.getlabel().'"/>';
  $xml .= $nl.'		<foxml:property NAME="info:fedora/fedora-system:def/model#ownerId" VALUE="'.$in['ownerId'].'"/>';
  $xml .= $nl.'	</foxml:objectProperties>';
  $xml .= $nl.'	<foxml:datastream CONTROL_GROUP="X" ID="DC" STATE="A" VERSIONABLE="true">';
  $xml .= $nl.'		<foxml:datastreamVersion ID="DC1.0" LABEL="Dublin Core Record" MIMETYPE="text/xml">';
  $xml .= $nl.'			<foxml:xmlContent>';
  $xml .= $nl.'				<oai_dc:dc xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">';
  $xml .= $nl.'					<dc:identifier>'.$in['pid'].'</dc:identifier>';
  $xml .= $nl.'				</oai_dc:dc>';
  $xml .= $nl.'			</foxml:xmlContent>';
  $xml .= $nl.'		</foxml:datastreamVersion>';
  $xml .= $nl.'	</foxml:datastream>';
  $xml .= $nl.'	<foxml:datastream CONTROL_GROUP="X" ID="RELS-EXT" STATE="A" VERSIONABLE="true">';
  $xml .= $nl.'		<foxml:datastreamVersion FORMAT_URI="info:fedora/fedora-system:FedoraRELSExt-1.0" ID="RELS-EXT1.0" LABEL="RDF Statements about this object" MIMETYPE="application/rdf+xml">';
  $xml .= $nl.'			<foxml:xmlContent>';
  $xml .= $nl.'				<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#" xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/" xmlns:fedora-model="info:fedora/fedora-system:def/model#">';
  $xml .= $nl.'					<rdf:description rdf:about="info:fedora/'.$in['pid'].'">';
  $xml .= $nl.'						<fedora:isMemberOfCollection rdf:resource="info:fedora/'.$in['parent']['pid'].'"></fedora:isMemberOfCollection>';
  $xml .= $nl.'						<fedora-model:hasModel rdf:resource="info:fedora/'.$in['model'].'"></fedora-model:hasModel>';
  $xml .= $nl.'					</rdf:description>';
  $xml .= $nl.'				</rdf:RDF>';
  $xml .= $nl.'			</foxml:xmlContent>';
  $xml .= $nl.'		</foxml:datastreamVersion>';
  $xml .= $nl.'	</foxml:datastream>';
  $xml .= $nl.'	<foxml:datastream ID="TN" STATE="A" CONTROL_GROUP="M" VERSIONABLE="true">';
  $xml .= $nl.'		<foxml:datastreamVersion ID="TN.0" LABEL="'.$in['primaryfile']['name'].'_TN.jpg" CREATED="'.$in['timestamp'].'" MIMETYPE="image/jpeg">';
  $xml .= $nl.'			<foxml:binaryContent>'.$in['primaryfile']['thumbnail'].'</foxml:binaryContent>';
  $xml .= $nl.'		</foxml:datastreamVersion>';
  $xml .= $nl.'	</foxml:datastream>';
  $xml .= $nl.'	<foxml:datastream CONTROL_GROUP="M" ID="MODS" STATE="A" VERSIONABLE="true">';
  $xml .= $nl.'		<foxml:datastreamVersion ID="MODS1.0" MIMETYPE="text/xml" LABEL="Descriptive Metadata Encoded in MODS">';
  $xml .= $nl.'			<foxml:xmlContent>';
  $xml .= $nl.'				<mods:modsCollection xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.loc.gov/mods/v3 http://www.loc.gov/standards/mods/v3/mods-3-4.xsd">';
  $xml .= $nl.'					<mods:mods>';

  	
  			# TITLE
  if ($in['meta']['eng']['title']['value'] != '')
  {
  	$xml .= $nl.'						<mods:titleInfo lang="eng">';
  	$xml .= $nl.'							<mods:title>'.fval($in['meta']['eng']['title']['value']).'</mods:title>';
  	$xml .= $nl.'						</mods:titleInfo>';
  }
  if ($in['meta']['ara']['title']['value'] != '')
  {
  	$xml .= $nl.'						<mods:titleInfo lang="ara">';
  	$xml .= $nl.'							<mods:title>'.fval($in['meta']['ara']['title']['value']).'</mods:title>';
  	$xml .= $nl.'						</mods:titleInfo>';
  }


  			# CREATOR / CREATOR AFFIL (FIRST ENTRY ONLY)
  if ($in['meta']['eng']['creator1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator1']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'							<mods:affiliation>'.fval($in['meta']['eng']['creatoraffil']['value']).'</mods:affiliation>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator2']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator2']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator3']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator3']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator4']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator4']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator5']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator5']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator6']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator6']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator7']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator7']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator8']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator8']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator9']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator9']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creator10']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creator10']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator1']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'							<mods:affiliation>'.fval($in['meta']['ara']['creatoraffil']['value']).'</mods:affiliation>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator2']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator2']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator3']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator3']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator4']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator4']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator5']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator5']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator6']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator6']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator7']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator7']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator8']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator8']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator9']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator9']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creator10']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="personal" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creator10']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creator</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }

  			
  			# CREATING ORG
  if ($in['meta']['eng']['creatingorg1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg1']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg2']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg2']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg3']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg3']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg4']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg4']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg5']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg5']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg6']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg6']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg7']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg7']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg8']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg8']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg9']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg9']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['eng']['creatingorg10']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="eng">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['eng']['creatingorg10']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg1']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg2']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg2']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg3']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg3']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg4']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg4']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg5']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg5']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg6']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg6']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg7']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg7']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg8']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg8']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg9']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg9']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }
  if ($in['meta']['ara']['creatingorg10']['value'] != '')
  {
  	$xml .= $nl.'						<mods:name type="corporate" lang="ara">';
  	$xml .= $nl.'							<mods:namePart>'.fval($in['meta']['ara']['creatingorg10']['value']).'</mods:namePart>';
  	$xml .= $nl.'							<mods:role>';
  	$xml .= $nl.'								<mods:roleTerm>Creating Organization</mods:roleTerm>';
  	$xml .= $nl.'							</mods:role>';
  	$xml .= $nl.'						</mods:name>';
  }

  			# ABSTRACT
  if ($in['meta']['eng']['abstract']['value'] != '')
  {
  	$xml .= $nl.'						<mods:abstract lang="eng">'.fval($in['meta']['eng']['abstract']['value']).'</mods:abstract>';
  }
  if ($in['meta']['ara']['abstract']['value'] != '')
  {
  	$xml .= $nl.'						<mods:abstract lang="ara">'.fval($in['meta']['ara']['abstract']['value']).'</mods:abstract>';
  }


  if ($in['meta']['eng']['abstract1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:abstract lang="eng">'.fval($in['meta']['eng']['abstract1']['value']).'</mods:abstract>';
  }
  if ($in['meta']['ara']['abstract1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:abstract lang="ara">'.fval($in['meta']['ara']['abstract1']['value']).'</mods:abstract>';
  }
  

  			# NOTE
  if ($in['meta']['eng']['note']['value'] != '')
  {
  	$xml .= $nl.'						<mods:note lang="eng">'.fval($in['meta']['eng']['note']['value']).'</mods:note>';
  }
  if ($in['meta']['ara']['note']['value'] != '')
  {
  	$xml .= $nl.'						<mods:note lang="ara">'.fval($in['meta']['ara']['note']['value']).'</mods:note>';
  }


  			# DOCUMENT NUMBER
  			if ($in['meta']['eng']['docno']['value'] != '')
  {
  	$xml .= $nl.'						<mods:identifier type="documentNumber">'.fval($in['meta']['eng']['docno']['value']).'</mods:identifier>';
  }
  if ($in['meta']['ara']['docno']['value'] != '')
  {
  	$xml .= $nl.'						<mods:identifier type="documentNumber">'.fval($in['meta']['ara']['docno']['value']).'</mods:identifier>';
  }

  	if ($in['meta']['eng']['docno1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:identifier type="documentNumber">'.fval($in['meta']['eng']['docno1']['value']).'</mods:identifier>';
  }
  if ($in['meta']['ara']['docno1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:identifier type="documentNumber">'.fval($in['meta']['ara']['docno1']['value']).'</mods:identifier>';
  }



  			# CONFERENCE
  if ($in['meta']['eng']['conferencename']['value'] != '' || $in['meta']['ara']['conferencename']['value'] != '')
  {
  	$xml .= $nl.'						<mods:relatedItem>';
  	if ($in['meta']['eng']['conferencename']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:name lang="eng" type="conference">';
  		$xml .= $nl.'								<mods:namePart>'.fval($in['meta']['eng']['conferencename']['value']).'</mods:namePart>';
  		$xml .= $nl.'							</mods:name>';
  	}
  	if ($in['meta']['ara']['conferencename']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:name lang="ara" type="conference">';
  		$xml .= $nl.'								<mods:namePart>'.fval($in['meta']['ara']['conferencename']['value']).'</mods:namePart>';
  		$xml .= $nl.'							</mods:name>';
  	}
  	$xml .= $nl.'						</mods:relatedItem>';
  }


  			 # JOURNAL NAME / NUMBER / DATE / PAGE RANGE / ISSN
  if ($_SESSION['meta']['eng']['journalname']['value'] != '' ||
  	$_SESSION['meta']['ara']['journalname']['value'] != '' ||
  	$_SESSION['meta']['eng']['journalno']['value'] != '' ||
  	$_SESSION['meta']['ara']['journalno']['value'] != '' ||
  	$_SESSION['meta']['eng']['journaldate']['value'] != '' ||
  	$_SESSION['meta']['ara']['journaldate']['value'] != '' ||
  	$_SESSION['meta']['eng']['journalpagerange']['value'] != '' ||
  	$_SESSION['meta']['ara']['journalpagerange']['value'] != '' ||
  	$_SESSION['meta']['eng']['journalissn']['value'] != '' ||
  	$_SESSION['meta']['ara']['journalissn']['value'] != '')
  {
  	$xml .= $nl.'						<mods:relatedItem type="host">';		
  	if ($_SESSION['meta']['eng']['journalname']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:titleInfo lang="eng">';
  		$xml .= $nl.'								<mods:title>'.fval($_SESSION['meta']['eng']['journalname']['value']).'</mods:title>';
  		$xml .= $nl.'							</mods:titleInfo>';
  	}
  	if ($_SESSION['meta']['ara']['journalname']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:titleInfo lang="ara">';
  		$xml .= $nl.'								<mods:title>'.fval($_SESSION['meta']['ara']['journalname']['value']).'</mods:title>';
  		$xml .= $nl.'							</mods:titleInfo>';
  	}
  	if ($_SESSION['meta']['eng']['journalno']['value'] != '' ||
  		$_SESSION['meta']['eng']['journaldate']['value'] != '' ||
  		$_SESSION['meta']['eng']['journalpagerange']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:part lang="eng">';
  		if ($_SESSION['meta']['eng']['journalno']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:detail type="volume">';
  			$xml .= $nl.'									<mods:number>'.fval($_SESSION['meta']['eng']['journalno']['value']).'</mods:number>';
  			$xml .= $nl.'								</mods:detail>';
  		}
  		if ($_SESSION['meta']['eng']['journaldate']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:date>'.fval($_SESSION['meta']['eng']['journaldate']['value']).'</mods:date>';
  		}			
  		if ($_SESSION['meta']['eng']['journalpagerange']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:extent>';
  			$xml .= $nl.'									<mods:list>'.fval($_SESSION['meta']['eng']['journaldate']['value']).'</mods:list>';
  			$xml .= $nl.'								</mods:extent>';
  		}			
  		$xml .= $nl.'							</mods:part>';
  	}
  	if ($_SESSION['meta']['ara']['journalno']['value'] != '' ||
  		$_SESSION['meta']['ara']['journaldate']['value'] != '' ||
  		$_SESSION['meta']['ara']['journalpagerange']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:part lang="ara">';
  		if ($_SESSION['meta']['ara']['journalno']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:detail type="volume">';
  			$xml .= $nl.'									<mods:number>'.fval($_SESSION['meta']['ara']['journalno']['value']).'</mods:number>';
  			$xml .= $nl.'								</mods:detail>';
  		}
  		if ($_SESSION['meta']['ara']['journaldate']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:date>'.fval($_SESSION['meta']['ara']['journaldate']['value']).'</mods:date>';
  		}			
  		if ($_SESSION['meta']['ara']['journalpagerange']['value'] != '')
  		{
  			$xml .= $nl.'								<mods:extent>';
  			$xml .= $nl.'									<mods:list>'.fval($_SESSION['meta']['ara']['journaldate']['value']).'</mods:list>';
  			$xml .= $nl.'								</mods:extent>';
  		}			
  		$xml .= $nl.'							</mods:part>';
  	}
  	if ($_SESSION['meta']['eng']['journalissn']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:identifier lang="eng" type="issn">'.fval($_SESSION['meta']['eng']['journalissn']['value']).'</mods:identifier>';
  	}
  	if ($_SESSION['meta']['ara']['journalissn']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:identifier lang="ara" type="issn">'.fval($_SESSION['meta']['ara']['journalissn']['value']).'</mods:identifier>';
  	}
  	$xml .= $nl.'						</mods:relatedItem>';
  }

  			# SUBJECT CLASSIFICATION
  if ($in['meta']['eng']['gemet']['value'] != '')
  {
  	$xml .= $nl.'						<mods:subject authority="TBD" lang="eng">';
  	$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['gemet']['value']).'</mods:topic>';
  	$xml .= $nl.'						</mods:subject>';
  }
  			
  			# KEYWORDS
  if ($in['meta']['eng']['keywords1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:subject authority="keyword" lang="eng">';
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords1']['value']).'</mods:topic>';

  	if ($in['meta']['eng']['keywords2']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords2']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords3']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords3']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords4']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords4']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords5']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords5']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords6']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords6']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords7']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords7']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords8']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords8']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords9']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords9']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['eng']['keywords10']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['eng']['keywords10']['value']).'</mods:topic>';
  	}

  	$xml .= $nl.'						</mods:subject>';
  }
  if ($in['meta']['ara']['keywords1']['value'] != '')
  {
  	$xml .= $nl.'						<mods:subject authority="keyword" lang="ara">';
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords1']['value']).'</mods:topic>';

  	if ($in['meta']['ara']['keywords2']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords2']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords3']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords3']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords4']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords4']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords5']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords5']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords6']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords6']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords7']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords7']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords8']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords8']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords9']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords9']['value']).'</mods:topic>';
  	}
  	if ($in['meta']['ara']['keywords10']['value'] != '')
  	{
  		$xml .= $nl.'							<mods:topic>'.fval($in['meta']['ara']['keywords10']['value']).'</mods:topic>';
  	}

  	$xml .= $nl.'						</mods:subject>';
  }


  			
  			# LANGUAGE
  if ($in['meta']['eng']['language']['value'] != '')
  {
  	$xml .= $nl.'						<mods:language lang="eng">';
  	$xml .= $nl.'							<mods:languageTerm>'.fval($in['meta']['eng']['language']['value']).'</mods:languageTerm>';
  	$xml .= $nl.'						</mods:language>';
  }
  if ($in['meta']['ara']['language']['value'] != '')
  {
  	$xml .= $nl.'						<mods:language lang="ara">';
  	$xml .= $nl.'							<mods:languageTerm>'.fval($in['meta']['ara']['language']['value']).'</mods:languageTerm>';
  	$xml .= $nl.'						</mods:language>';
  }	

  			# PUBLISHER / DATE PUBLISH / DATE ORIGINAL 
  $xml .= $nl.'						<mods:originInfo lang="eng">';


  	if ($in['meta']['eng']['publisher']['value'] != '')
  {
  	$xml .= $nl.'							<mods:publisher>'.fval($in['meta']['eng']['publisher']['value']).'</mods:publisher>';
  }
  if ($in['meta']['ara']['publisher']['value'] != '')
  {
  	$xml .= $nl.'							<mods:publisher>'.fval($in['meta']['ara']['publisher']['value']).'</mods:publisher>';
  }



  		if ($in['meta']['eng']['dateoriginalyear']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['eng']['dateoriginalyear']['value']).'</mods:dateCreated>';
  }
  if ($in['meta']['ara']['dateoriginalyear']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['ara']['dateoriginalyear']['value']).'</mods:dateCreated>';
  }
  
  			if ($in['meta']['eng']['dateoriginalyear1']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['eng']['dateoriginalyear1']['value']).'</mods:dateCreated>';
  }
  if ($in['meta']['ara']['dateoriginalyear1']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['ara']['dateoriginalyear1']['value']).'</mods:dateCreated>';
  }


  		if ($in['meta']['eng']['dateoriginalyear']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['eng']['dateoriginalyear']['value']).'</mods:dateCreated>';
  }
  if ($in['meta']['ara']['dateoriginalyear']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateCreated>'.fval($in['meta']['ara']['dateoriginalyear']['value']).'</mods:dateCreated>';
  }
  
  			if ($in['meta']['eng']['dateoriginal']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateIssued>'.fval($in['meta']['eng']['dateoriginal']['value']).'</mods:dateIssued>';
  }
  if ($in['meta']['ara']['dateoriginal']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateIssued>'.fval($in['meta']['ara']['dateoriginal']['value']).'</mods:dateIssued>';
  }

  			if ($in['meta']['eng']['dateoriginal1']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateIssued>'.fval($in['meta']['eng']['dateoriginal1']['value']).'</mods:dateIssued>';
  }
  if ($in['meta']['ara']['dateoriginal1']['value'] != '')
  {
  	$xml .= $nl.'							<mods:dateIssued>'.fval($in['meta']['ara']['dateoriginal1']['value']).'</mods:dateIssued>';
  }
  
  $xml .= $nl.'						</mods:originInfo>';

  			
  			 # RIGHTS
  if ($in['meta']['eng']['rights']['value'] != '')
  {
  	$xml .= $nl.'						<mods:accessCondition type="use and reproduction" lang="eng" displayLabel="Rights">'.$in['meta']['eng']['rights']['value'].'</mods:accessCondition>';
  }
  if ($in['meta']['ara']['rights']['value'] != '')
  {
  	$xml .= $nl.'						<mods:accessCondition type="use and reproduction" lang="ara" displayLabel="Rights">'.$in['meta']['ara']['rights']['value'].'</mods:accessCondition>';
  }


  			# GEO CARTOGRAPHIC / GEO COVERAGE
  if ($in['meta']['eng']['geocartographic']['value'] != '' || $in['meta']['eng']['geocoverage']['value'] != '')
  {
  	$xml .= $nl.'						<mods:subject lang="eng">';
  	$xml .= $nl.'							<mods:cartographics>';
  	$xml .= $nl.'								<mods:coordinates>'.fval($in['meta']['eng']['geocartographic']['value']).'</mods:coordinates>';
  	$xml .= $nl.'							</mods:cartographics>';
  	$xml .= $nl.'							<mods:geographicCode>'.fval($in['meta']['eng']['geohierarchical']['value']).'</mods:geographicCode>';
  	$xml .= $nl.'						</mods:subject>';
  }
  if ($in['meta']['ara']['geocartographic']['value'] != '' || $in['meta']['ara']['geocoverage']['value'] != '')
  {
  	$xml .= $nl.'						<mods:subject lang="ara">';
  	$xml .= $nl.'							<mods:cartographics>';
  	$xml .= $nl.'								<mods:coordinates>'.fval($in['meta']['ara']['geocartographic']['value']).'</mods:coordinates>';
  	$xml .= $nl.'							</mods:cartographics>';
  	$xml .= $nl.'							<mods:geographicCode>'.fval($in['meta']['ara']['geohierarchical']['value']).'</mods:geographicCode>';
  	$xml .= $nl.'						</mods:subject>';
  }

  
  			# EXTENT
  if ($in['meta']['eng']['extent']['value'] != '')
  {
  	$xml .= $nl.'						<mods:physicalDescription lang="eng">';
  	$xml .= $nl.'							<mods:extent>'.fval($in['meta']['eng']['extent']['value']).'</mods:extent>';
  	$xml .= $nl.'						</mods:physicalDescription>';
  }
  if ($in['meta']['ara']['extent']['value'] != '')
  {
  	$xml .= $nl.'						<mods:physicalDescription lang="ara">';
  	$xml .= $nl.'							<mods:extent>'.fval($in['meta']['ara']['extent']['value']).'</mods:extent>';
  	$xml .= $nl.'						</mods:physicalDescription>';
  }


  			# OBJECT TYPE
  $xml .= $nl.'						<mods:genre authority="objectType" lang="eng">'.fval(getobjecttype()).'</mods:genre>';


  			# METADATA CONTACT
  $xml .= $nl.'						<mods:name lang="eng">';
  $xml .= $nl.'							<mods:namePart>'.fval($in['contact']).'</mods:namePart>';
  $xml .= $nl.'						<mods:role>';
  $xml .= $nl.'							<mods:roleTerm>Metadata Contact</mods:roleTerm>';
  $xml .= $nl.'						</mods:role>';
  $xml .= $nl.'						</mods:name>';



  $xml .= $nl.'					</mods:mods>';
  $xml .= $nl.'				</mods:modsCollection>';
  $xml .= $nl.'			</foxml:xmlContent>';
  $xml .= $nl.'		</foxml:datastreamVersion>';
  $xml .= $nl.'	</foxml:datastream>';


  			# PRIMARY FILE
  $xml .= $nl.'	<foxml:datastream CONTROL_GROUP="M" ID="OBJ" STATE="A" VERSIONABLE="true">';
  $xml .= $nl.'		<foxml:datastreamVersion FORMAT_URI="unknown" ID="OBJ.0" LABEL="'.fval($arr['primaryfile']['name']).'" MIMETYPE="'.getmime($arr['primaryfile']['name']).'">';
  $xml .= $nl.'			<foxml:binaryContent>'.$in['binary'].'</foxml:binaryContent>';
  $xml .= $nl.'		</foxml:datastreamVersion>';
  $xml .= $nl.'	</foxml:datastream>';


  			# SECONDARY FILES
  foreach($in['secondaryfiles'] as $obj) {
  	$xml .= $nl.'	<foxml:datastream CONTROL_GROUP="M" ID="'.$obj['id'].'" STATE="A" VERSIONABLE="true">';
  	$xml .= $nl.'		<foxml:datastreamVersion FORMAT_URI="unknown" ID="'.$obj['id'].'.0" LABEL="'.fval($obj['name']).'" MIMETYPE="'.getmime($obj['name']).'">';
  	$xml .= $nl.'			<foxml:binaryContent>'.$obj['binary'].'</foxml:binaryContent>';
  	$xml .= $nl.'		</foxml:datastreamVersion>';
  	$xml .= $nl.'	</foxml:datastream>';
  }

  $xml .= $nl.'</foxml:digitalObject>';

  $xml .= $nl.'';

  return $xml;
}
