<?php
/**
 * @file
 * This is the template file for the person object page.
 *
 * Here we build an html page using the variables passed in by
 * the islandora_bioinformatics_base_preprocess_islandora_person
 * function.  Elements such as labels and buttons can be added here
 */

/**
 * 	Written by Quintin Dawson, based off of Islandora_Porcus solution pack
 */

//$islandora_content = $islandora_object['PERSON']->content;
//$path = drupal_get_path('module', 'islandora_bioinformatics_base');

/*echo "<pre>";
print_r($variables['project']);
echo "</pre>";*/
?>

<style>
  .project-info {
    float: left;
    position: relative;
    top: 8px;
    margin-right: 20px;
  }

  .project-pic {
    float: left;
    position: relative;
    top: 8px;
    border: none;
  }

  .project-info table {
    border: none;
  }

  .project-info-label {
    font-weight: bold;
    font-size: 14px;
    width: 180px;
  }

  .project-info-heading {
    font-weight: bold;
    font-size: 14px;
    width: 180px;
    border: none;
    background: none;
    text-decoration:underline;
  }
</style>

<div class="project-info">
  <table>
    <tbody>
    <?php if (isset ($variables['img_url'])):?><tr><td><img src="<?php print $variables['img_url'] ?>" width="250"></td></tr><?php endif; ?>
    </tbody>
  </table>
</div>
<div class="project-info">
  <table>
    <tbody>
    <tr><th class="project-info-heading">Project Information</th><th class="m-btn green"><?php print l('Edit', "islandora/edit_form/{$variables['islandora_object']->id}/MADS");?></th></tr>
    <tr><td class="project-info-label">Title:</td><?php if (isset ($variables['project']['title'])):?><td><?php print $variables['project']['title'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Organization:</td><?php if (isset ($variables['project']['organization'])):?><td><?php print $variables['project']['organization'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">City:</td><?php if (isset ($variables['project']['address']['city'])):?><td><?php print $variables['project']['address']['city'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Street:</td><?php if (isset ($variables['project']['address']['street'])):?><td><?php print $variables['project']['address']['street'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Country:</td><?php if (isset ($variables['project']['address']['country'])):?><td><?php print $variables['project']['address']['country'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Postal Code:</td><?php if (isset ($variables['project']['address']['postcode'])):?><td><?php print $variables['project']['address']['postcode'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Email Address:</td><?php if (isset ($variables['project']['email'])):?><td><?php print $variables['project']['email'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Phone Number:</td><?php if (isset ($variables['project']['phone'])):?><td><?php print $variables['project']['phone'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">Start Date:</td><?php if (isset ($variables['project']['startdate'])):?><td><?php print $variables['project']['startdate'] ?></td><?php endif; ?></tr>
    <tr><td class="project-info-label">End Date:</td><?php if (isset ($variables['project']['enddate'])):?><td><?php print $variables['project']['enddate'] ?></td><?php endif; ?></tr>
    </tbody>
  </table>
</div>

