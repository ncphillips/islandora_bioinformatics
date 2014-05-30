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
print_r($variables["person"]);
echo "</pre>";*/
?>

<style>
  .person-info {
    float: left;
    position: relative;
    top: 8px;
    margin-right: 20px;
  }

  .person-pic {
    float: left;
    position: relative;
    top: 8px;
    border: none;
  }

  .person-info table {
    border: none;
  }

  .person-info-label {
    font-weight: bold;
    font-size: 14px;
    width: 180px;
  }

  .person-info-heading {
    font-weight: bold;
    font-size: 14px;
    width: 180px;
    border: none;
    background: none;
    text-decoration:underline;
  }
</style>

<div class="person-info">
  <table>
    <tbody>
    <?php if (isset ($variables['img_url'])):?><tr><td><img src="<?php print $variables['img_url'] ?>" width="250"></td></tr><?php endif; ?>
    </tbody>
  </table>
</div>
<div class="person-info">
  <table>
    <tbody>
    <tr><th class="person-info-heading">Personal Information</th></tr>
    <tr><td class="person-info-label">Title:</td><?php if (isset ($variables['person']['title'])):?><td><?php print $variables['person']['title'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">First Name:</td><?php if (isset ($variables['person']['name']['first'])):?><td><?php print $variables['person']['name']['first'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Last Name:</td><?php if (isset ($variables['person']['name']['last'])):?><td><?php print $variables['person']['name']['last'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Birth Date: Name:</td><?php if (isset ($variables['person']['name']['birthdate'])):?><td><?php print $variables['person']['name']['birthdate'] ?></td><?php endif; ?></tr>
    </tbody>
  </table>
</div>
<div class="person-info">
  <table>
    <tbody>
    <tr><th class="person-info-heading">Contact Information</th></tr>
    <tr><td class="person-info-label">Phone Number:</td><?php if (isset ($variables['person']['contact']['phone'])):?><td><?php print $variables['person']['contact']['phone'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Email Address:</td><?php if (isset ($variables['person']['contact']['email'])):?><td><?php print $variables['person']['contact']['email'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Organization:</td><?php if (isset ($variables['person']['contact']['organization'])):?><td><?php print $variables['person']['contact']['organization'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Position:</td><?php if (isset ($variables['person']['contact']['position'])):?><td><?php print $variables['person']['contact']['position'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Address:</td><?php if (isset ($variables['person']['contact']['address'])):?><td><?php print $variables['person']['contact']['address'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Start Date:</td><?php if (isset ($variables['person']['contact']['startDate'])):?><td><?php print $variables['person']['contact']['startDate'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">End Date:</td><?php if (isset ($variables['person']['contact']['endDate'])):?><td><?php print $variables['person']['contact']['endDate'] ?></td><?php endif; ?></tr>
    <tr><td class="person-info-label">Website:</td><?php if (isset ($variables['person']['contact']['url'])):?><td><?php print $variables['person']['contact']['url'] ?></td><?php endif; ?></tr>
    </tbody>
  </table>
</div>
<div class="person-info float-right m-btn green">
  <?php print l('Edit', "islandora/edit_form/{$variables['islandora_object']->id}/MADS");?>
</div>
