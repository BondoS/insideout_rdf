<?php

/**
* Create a taxonomy term and return the tid.
* @param int $vid
* vocabulary id.
* @param array $term
* term contains all its features/fields as key value pairs.
* @return int
* term id.
*/
function create_rdf_taxonomy_term( $vid, $term) {
  $newTerm = new stdClass();
  $newTerm->vid = $vid;
  $newTerm->name = $term['prefLabel'];
  $newTerm->field_skos_definition['und'][0]['value'] =  $term['definition'];
  $newTerm->field_identifier['und'][0]['value'] = $term['identifier'];
  $newTerm->field_skos_notation['und'][0]['value'] = $term['notation'];
  $newTerm->field_uri['und'][0]['url'] = $term['uri'];
  taxonomy_term_save($newTerm);
  return $newTerm->tid;
}

/**
* Import triples from RDF uri source then save them as terms.
*/
function importTermsFromRdfSource(){
  global $base_url;
  $vocabMachineName = variable_get('insideout_rdf_vocabulary_machine_name');
  $vocab = taxonomy_vocabulary_machine_name_load($vocabMachineName);
  $uri = 'http://dati.gov.it/onto/controlledvocabulary/InteractivityLevel';

  // Load rdf_format.inc file from the taxonomy_xml module.
  module_load_include('inc','taxonomy_xml','formats/rdf_format');

  // Import rdf file data to a php string.
  $rdfString = file_get_contents($uri);
  // Get An array set of triples.
  $result = taxonomy_xml_rdf_parse_data_into_index($rdfString,$uri);

  $count = 0;
  // specify each skos feature value, save them as key value pairs, in order to easily save them as terms.
  foreach ($result as $key => $value) {
    //skip ConceptScheme and loop through the remaining Concepts.
    if($key != $uri){
      $term = array();
      $term['uri'] = $key;
      foreach ($value as $key => $innerValue) {
        @list($resource_url, $anchor) = explode('#', $key);
        // if anchor is empty, get feature name from url.
        if(empty($anchor)){
          $anchor = end((explode('/', $resource_url)));
        }
        // here is where i can't specify the language. because parser doesn't declare the language. i think i need to write my own parser !.
        $term[$anchor] = $innerValue[0];
      }
      // Save new term.
      create_rdf_taxonomy_term( $vocab->vid, $term);
      $count++;
    }
  }
  // Give user feedback.
  drupal_set_message( $count . t(' Terms imported successfully, you can edit them ') . '<a href="'.$base_url.'/admin/structure/taxonomy/'.$vocabMachineName.'">'.t('here').'</a>');
}
