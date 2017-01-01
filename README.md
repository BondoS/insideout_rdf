# insideout_rdf

This module has been developed for Drupal 7.x for interview task.  

Main functionality is to import RDF data from external source 'URI' then parse the data and save it in Taxonomy terms.

You should install taxonomy_xml module and import ARC2 library before enabling this module, otherwise it won't work.

How it works.
- install the module
- enable the module
- Under structure admin menu tab, you will find "Import RDF to Taxonomy", hit it.
- You should receive a success message and you can now view your imported terms.

Notes.
- URI is hardcoded in the code. I might change it to be editable in the module configuration page, but no need to do it for now.
