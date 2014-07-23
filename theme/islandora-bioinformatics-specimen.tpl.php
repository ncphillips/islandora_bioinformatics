<?php
/**
 * @file
 * This is the template file for the specimen object page.
 *
 * Here we build an html page using the variables passed in by
 * the islandora_bioinformatics_specimen_preprocess_islandora_specimen
 * function.  Elements such as labels and buttons can be added here
 *
 */
?>
<style>
    .specimen-info {
        width: 33%;
        float: left;
        position: relative;
        top: 8px;
    }

    .specimen-info-label {
        font-weight: bold;
        font-size: 14px;
        width: 180px;
    }

    .specimen-info table {
        border: none;
    }

    .specimen-info-heading {
        font-weight: bold;
        font-size: 14px;
        width: 180px;
        border: none;
        background: none;
        text-decoration:underline;
    }
    .rel-proj-btn{
      position: relative;
      color: red;
      top: 1px;
      float:left;
    }
</style>

<div class="m-btn green">
  <?php print l('Edit', "islandora/edit_form/{$variables['islandora_object']->id}/EML");?>
</div>
<div class="">
  <div class="specimen-info">
    <table>
      <tbody>
        <tr><th class="specimen-info-heading">Taxonomic Information</th></tr>
        <tr><td class="specimen-info-label">Phylum: </td><td><?php print $variables['eml']['taxonomicCoverage']['phylum'] ?></td></tr>
        <tr><td class="specimen-info-label">Subphylum: </td><td><?php print $variables['eml']['taxonomicCoverage']['subphylum'] ?></td></tr>
        <tr><td class="specimen-info-label">Class: </td><td><?php print $variables['eml']['taxonomicCoverage']['class'] ?></td></tr>
        <tr><td class="specimen-info-label">Order: </td><td><?php print $variables['eml']['taxonomicCoverage']['order'] ?></td></tr>
        <tr><td class="specimen-info-label">Family: </td><td><?php  print $variables['eml']['taxonomicCoverage']['family'] ?></td></tr>
        <tr><td class="specimen-info-label">Genus: </td><td><?php  print $variables['eml']['taxonomicCoverage']['genus'] ?></td></tr>
        <tr><td class="specimen-info-label">Species: </td><td><?php  print $variables['eml']['taxonomicCoverage']['species'] ?></td></tr>
      </tbody>
    </table>
  </div>
</div>

<div class="related-projects">
  <table>
        <tbody>
        <tr>
          <th>
            <div class="m-btn green"><?php print l('Manage Related Projects', "islandora/object/{$variables['object_id']}/manage_lab_object_projects") ?></div>
          </th>
        </tr>
          <tr>
            <th class="specimen-info-heading">Related Projects </th>
          </tr>
        </tbody>
            <?php if(isset($variables['related_projects'])): ?>
                <?php foreach($variables['related_projects'] as $key => $project): ?>
                    <tr><td class="specimen-info-label"><?php print l($project, "islandora/object/{$key}") ?> </td></tr>
                <?php endforeach; ?>
            <?php endif; ?>
        </tbody>
    </table>
</div>

