<?php
function hasfield($objtype, $field)
{
  if (!isset($_SESSION['objtype-array'])) { buildarray(); }

  $arr = $_SESSION['objtype-array'];
  $ret = in_array($field, $arr[$objtype]);
  unset($arr);
  
  return $ret;	
}

function buildfieldarray()
{
  $arr = array();
  $arr['technicalreport'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract','note','docno','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginal','geocartographic','geohierarchical','extent');
  $arr['presentation'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','note','docno1','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear1','dateoriginal1','geocartographic','geohierarchical','extent','conferencename');
  $arr['researcharticle'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract1','note','docno','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginal','geocartographic','geohierarchical','extent','conferencename','journalname','journalno','journaldate','journalpagerange','journalissn');
  $arr['technologyreport'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract','note','docno','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginal','geocartographic','geohierarchical','extent');
  $arr['thesis'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract','note','docno','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginal','geocartographic','geohierarchical','extent');
  $arr['map'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract','note','docno','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginal','geocartographic','geohierarchical','extent');
  $arr['all'] = array('title','creator1','creator2','creator3','creator4','creator5','creator6','creator7','creator8','creator9','creator10','creatoraffil','creatingorg1','creatingorg2','creatingorg3','creatingorg4','creatingorg5','creatingorg6','creatingorg7','creatingorg8','creatingorg9','creatingorg10','gemet','abstract','abstract1','note','docno','docno1','keywords1','keywords2','keywords3','keywords4','keywords5','keywords6','keywords7','keywords8','keywords9','keywords10','language','publisher','rights','dateoriginalyear','dateoriginalyear1','dateoriginal','dateoriginal1','geocartographic','geohierarchical','extent','conferencename','journalname','journalno','journaldate','journalpagerange','journalissn');
  $_SESSION['objtype-array'] = $arr;
  return true;
}

function getfieldoptions($field, $lang)
{
  switch ($field)
  {
  	### Language
  	case 'language':
  		if ($lang == 'eng') {
  			$ret = array('' => '',
  						 'Arabic' => 'Arabic',
  						 'English' => 'English',
  						 'Arabic and English' => 'Arabic and English');
  		}
  		else if ($lang == 'ara') {
  			$ret = array('' => '',
  						 'العربية' => 'العربية',
  						 'الإنجليزية' => 'الإنجليزية',
  						 'العربية والإنجليزية' => 'العربية والإنجليزية');
  		}
  		break;

  	case 'publisher':
  		if ($lang == 'eng') {
  			$ret = array("",
  						"Ministry of Oil - Iraq",
  						"Ministry of Finance - Iraq",
  						"Ministry of Interior - Iraq",
  						"Ministry of Defense - Iraq",
  						"Ministry of Electricity - Iraq",
  						"Ministry of Higher Education and Scientific Research - Iraq",
  						"Ministry of Industry and Minerals",
  						"Ministry of Planning - Iraq",
  						"Ministry of Environment - Iraq",
  						"Ministry of Agriculture - Iraq",
  						"Ministry of Education - Iraq",
  						"Ministry of Construction and Housing - Iraq",
  						"Ministry of Health - Iraq",
  						"Ministry of Communications - Iraq",
  						"Ministry of Water Resource - Iraq",
  						"Ministry of State for Tourism and Antiquities  - Iraq",
  						"National Investment Commission - Iraq",
  						"Communication and Media Commission - Iraq",
  						"Iraqi Radioactive Sources Regularity Authority",
  						"Prime Minister Advisory Committee (PMAC) - Iraq",
  						"Renewable Energy Dir.",
  						"Industrial R&D Dir.",
  						"Space Technology & Communication Dir.",
  						"Information Technology Dir.",
  						"Agricultural Dir.",
  						"Environment  & Water Treatment Dir.",
  						"Materials Research Dir.",
  						"Iraqi Atomic Energy Commission",
  						"National Monitoring Commission",
  						"MoST HQ",
  						"University of Baghdad",
  						"University of Mustanseriya",
  						"University of Basrah",
  						"University of Mousel",
  						"University of Technology",
  						"University of Kufa",
  						"University of Tikrit",
  						"University of Al-Qadisiya",
  						"University of Al-Anbar",
  						"University of Babylon",
  						"University of Al-Nahrain",
  						"University of Deyala",
  						"University of Karbala",
  						"University of Thi-Qar",
  						"University of Kir Kuk",
  						"University of Wassit",
  						"University of Mysaan",
  						"University of Al-Muthanna",
  						"University of sulaymaniyah",
  						"University of Salah Al-Din",
  						"University of Duhok",
  						"University of Koya",
  						"hawler Medical university");
  			}
  			else if ($lang == 'ara') {
  				$ret = array("",
  					"وزارة النفط - العراق",
  					"وزارة المالية - العراق",
  					"وزارة الداخلية - العراق",
  					"وزارة الدفاع - العراق",
  					"وزارة الكهرباء - العراق",
  					"وزارة التعليم العالي والبحث العلمي - العراق",
  					"وزارة الصناعة والمعادن - العراق",
  					"وزارة التخطيط - العراق ",
  					"وزارة البيئة - العراق",
  					"وزارة الزراعة - العراق",
  					"وزارة التربية - العراق",
  					"وزارة الاعمار والاسكان - العرا",
  					"وزارة الصحة - العراق",
  					"وزارة الاتصالات - العرا",
  					"وزارة الموارد المائية - العراق",
  					"وزارة السياحة والآثار - العرا",
  					"الهيئة الوطنية للإستثمار - العرا",
  					"هيئة الإعلام والإتصالات - العرا",
  					"الهيئة العراقية للسيطرة على       المصادر المشعة",
  					"هيئة المستشارين - العراق",
  					"دائرة  الطاقات المتجددة",
  					"دائرة البحث والتطوير الصناعي",
  					"دائرة تكنولوجيا الفضاء والاتصالات",
  					"دائرة تكنولوجيا المعلومات",
  					"دائرة البحوث الزراعية",
  					"دائرة البيئة ومعالجة المياة",
  					"دائرة بحوث المواد",
  					"هيئة الطاقة الذرية العراقية",
  					"هيئة الرقابة الوطنية",
  					"مقر الوزارة",
  					"جامعة بغداد",
  					"الجامعة المستنصرية",
  					"جامعة البصرة",
  					"جامعة الموصل",
  					"الجامعة التكنولوجية",
  					"جامعة الكوفة",
  					"جامعة تكريت",
  					"جامعة القادسية",
  					"جامعة الانبار",
  					"جامعة بابل",
  					"جامعة النهرين",
  					"جامعة ديالى",
  					"جامعة كربلاء",
  					"جامعة ذي قار",
  					"جامعة كركوك",
  					"جامعة واسط",
  					"جامعة ميسان",
  					"جامعة المثنى",
  					"جامعة السليمانية",
  					"جامعة صلاح الدين",
  					"جامعة دهوك",
  					"جامعة كويه",
  					"جامعة هولير الطبية");
  			}
  
  		break;
  	default:
  		$ret = array('','1','2','3');
  		break;
  }

  	return $ret;
}

function istir_forms_initmetadata($force = false) {
  if (!isset($_SESSION['meta']) || $force == true)
  {
  	initupload();
  	buildfieldarray();
  	$_SESSION['meta'] = array();
  	$_SESSION['meta']['object_type'] = '';
  	$_SESSION['meta']['subjectcollection'] = '';
  	$_SESSION['meta']['publicationstatus'] = '';
  	
  	### Title
  	$_SESSION['meta']['ara']['title'] = array(
  		'description' => 'عنوان الوثيقة', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['title'] = array(
  		'description' => 'Title',
  		'value' => '',
  		'required' => true,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Author
  	$_SESSION['meta']['ara']['creator1'] = array(
  		'description' => 'المؤلف / المؤلفين', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  			$_SESSION['meta']['ara']['creator2'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator3'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator4'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator5'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator6'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator7'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator8'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator9'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creator10'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);

  	$_SESSION['meta']['eng']['creator1'] = array(
  		'description' => 'Author(s)',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);
  			$_SESSION['meta']['eng']['creator2'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator3'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator4'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator5'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator6'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator7'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator8'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator9'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creator10'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);

  	### Creator Affiliation
  	$_SESSION['meta']['ara']['creatoraffil'] = array(
  		'description' => 'جهة انتساب المؤلف الرئيسي', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['creatoraffil'] = array(
  		'description' => 'Author Affiliation',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Creating Organization		
  	$_SESSION['meta']['ara']['creatingorg1'] = array(
  		'description' => 'جهة اعداد الوثيقة', 
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  			$_SESSION['meta']['ara']['creatingorg2'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg3'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg4'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg5'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg6'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg7'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg8'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg9'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['creatingorg10'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);

  	$_SESSION['meta']['eng']['creatingorg1'] = array(
  		'description' => 'Creating Organization',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  			$_SESSION['meta']['eng']['creatingorg2'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg3'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);

  			$_SESSION['meta']['eng']['creatingorg4'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);

  			$_SESSION['meta']['eng']['creatingorg5'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg6'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg7'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg8'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg9'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['creatingorg10'] = array(
  				'value' => '',
  				'required' => false,
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);


  	### GEMET Subject Category
  	$_SESSION['meta']['ara']['gemet'] = array(
  		'description' => 'التصنيف القياسي للموضوع', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['gemet'] = array(
  		'description' => 'Formal Subject Classification',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  	### Abstract
  	$_SESSION['meta']['ara']['abstract'] = array(
  		'description' => 'خلاصة الوثيقة', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textarea',
  		'multiple' => false,
  		'length' => 5000,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['abstract'] = array(
  		'description' => 'Summary',
  		'value' => '',
  		'required' => true,
  		'type' => 'textarea',
  		'multiple' => false,
  		'length' => 5000,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  	$_SESSION['meta']['ara']['abstract1'] = array(
  		'description' => 'مستخلص', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textarea',
  		'multiple' => false,
  		'length' => 5000,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['abstract1'] = array(
  		'description' => 'Abstract',
  		'value' => '',
  		'required' => true,
  		'type' => 'textarea',
  		'multiple' => false,
  		'length' => 5000,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Note
  	$_SESSION['meta']['ara']['note'] = array(
  		'description' => 'ملاحظات', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['note'] = array(
  		'description' => 'Note',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Document/Report Number
  	$_SESSION['meta']['ara']['docno'] = array(
  		'description' => 'رقم الوثيقة / التقرير', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['docno'] = array(
  		'description' => 'Document/Report Number',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  	$_SESSION['meta']['ara']['docno1'] = array(
  		'description' => 'رقم الوثيقة / العرض', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['docno1'] = array(
  		'description' => 'Document/Report Number',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  	### Keywords
  	$_SESSION['meta']['ara']['keywords1'] = array(
  		'description' => 'كلمات مفتاحية حرة', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  			$_SESSION['meta']['ara']['keywords2'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords3'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords4'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords5'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords6'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords7'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords8'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords9'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);
  			$_SESSION['meta']['ara']['keywords10'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'ara-txt'),
  			);

  	$_SESSION['meta']['eng']['keywords1'] = array(
  		'description' => 'Uncontrolled Keywords',
  		'value' => '',
  		'required' => true,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);
  			$_SESSION['meta']['eng']['keywords2'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords3'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords4'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords5'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords6'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords7'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords8'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords9'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);
  			$_SESSION['meta']['eng']['keywords10'] = array(
  				'value' => '', 
  				'required' => false, 
  				'type' => 'textfield',
  				'multiple' => true,
  				'attributes' => array('class' => 'eng-txt'),
  			);

  	### Language
  	$_SESSION['meta']['ara']['language'] = array(
  		'description' => 'لغة الوثيقة', 
  		'required' => true, 
  		'type' => 'select',
  		'options' => getfieldoptions('language','ara'),
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-opt'),
  	);
  	$_SESSION['meta']['eng']['language'] = array(
  		'description' => 'Language',
  		'required' => true,
  		'type' => 'select',
  		'options' => getfieldoptions('language','eng'),
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-opt'),
  	);

  	### Publisher
  	$_SESSION['meta']['ara']['publisher'] = array(
  		'description' => 'الناشر', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['publisher'] = array(
  		'description' => 'Publisher',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Rights
  	$_SESSION['meta']['ara']['rights'] = array(
  		'description' => 'الحقوق المتعلقة بالوثيقة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['rights'] = array(
  		'description' => 'Rights',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Date of Original Year
  	$_SESSION['meta']['ara']['dateoriginalyear'] = array(
  		'description' => 'سنة الإصدار الإصلي', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['dateoriginalyear'] = array(
  		'description' => 'Year of Original Date',
  		'value' => '',
  		'required' => true,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	$_SESSION['meta']['ara']['dateoriginalyear1'] = array(
  		'description' => 'سنة تقديم العرض', 
  		'value' => '', 
  		'required' => true, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['dateoriginalyear1'] = array(
  		'description' => 'Year of Original Date',
  		'value' => '',
  		'required' => true,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  	### Date Original
  	$_SESSION['meta']['ara']['dateoriginal'] = array(
  		'description' => 'التاريخ الأصلي للوثيقة',
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['dateoriginal'] = array(
  		'description' => 'Date Original',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	$_SESSION['meta']['ara']['dateoriginal1'] = array(
  		'description' => 'تاريخ تقديم العرض',
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['dateoriginal1'] = array(
  		'description' => 'Date Original',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);



  	### Geographic Coverage - Cartographic (Coordinates)
  	$_SESSION['meta']['ara']['geocartographic'] = array(
  		'description' => 'التغطية الجغرافية / الكاتوغرافية',
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['geocartographic'] = array(
  		'description' => 'Geographic Coverage - Cartographic',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Geographic Coverage - Country/State/City/Town
  	$_SESSION['meta']['ara']['geohierarchical'] = array(
  		'description' => 'التغطية الجغرافية - الدولة / الولاية / المدينة / البلدة',
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['geohierarchical'] = array(
  		'description' => 'Geographic Coverage  Hierarchical',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => true,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Page Count/Size/Issue
  	$_SESSION['meta']['ara']['extent'] = array(
  		'description' => 'حجم الوثيقة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['extent'] = array(
  		'description' => 'Extent',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Conference Name
  	$_SESSION['meta']['ara']['conferencename'] = array(
  		'description' => 'اسم المؤتمر/ ورشة العمل', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['conferencename'] = array(
  		'description' => 'Conference/Workshop Name',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Journal Name
  	$_SESSION['meta']['ara']['journalname'] = array(
  		'description' => 'اسم المجلة التي نشرت فيها الوثيقة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['journalname'] = array(
  		'description' => 'Journal Name',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Journal Volume/Number
  	$_SESSION['meta']['ara']['journalno'] = array(
  		'description' => 'رقم المجلد / العدد للمجلة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['journalno'] = array(
  		'description' => 'Journal Volume/Number',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Journal Date
  	$_SESSION['meta']['ara']['journaldate'] = array(
  		'description' => 'تاريخ اصدار المجلة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['journaldate'] = array(
  		'description' => 'Journal Date',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Journal Page Range
  	$_SESSION['meta']['ara']['journalpagerange'] = array(
  		'description' => 'نطاق صفحات الوثيقة ضمن المجلة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['journalpagerange'] = array(
  		'description' => 'Page Range',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);

  	### Journal ISSN
  	$_SESSION['meta']['ara']['journalissn'] = array(
  		'description' => 'الرقم التسلسلي الدولي للمجلة', 
  		'value' => '', 
  		'required' => false, 
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'ara-txt'),
  	);
  	$_SESSION['meta']['eng']['journalissn'] = array(
  		'description' => 'ISSN',
  		'value' => '',
  		'required' => false,
  		'type' => 'textfield',
  		'multiple' => false,
  		'attributes' => array('class' => 'eng-txt'),
  	);


  }
}
